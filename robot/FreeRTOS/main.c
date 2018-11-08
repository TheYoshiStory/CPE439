/* Xilinx includes. */
#include "xparameters.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "timer_ps.h"
#include "xiicps.h"
#include "xgpio.h"
#include "platform.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <limits.h>

#define CODEC_ADDR 0b00011010
#define IIC_SCLK_RATE 100000

static void prvSetupHardware(void);
extern void vPortInstallFreeRTOSVectorTable(void);

void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationTickHook(void);

XScuWdt xWatchDogInstance;
XScuGic xInterruptController;

void sample_task(void *params)
{
    for (;;);
}

void RunTasks()
{
    /*
     * Start the two tasks as described in the comments at the top of this
     * file.
     */
    xTaskCreate (sample_task,                        /* The function that implements the task. */
                "BarDisplay",                        /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                configMINIMAL_STACK_SIZE,            /* The size of the stack to allocate to the task. */
                NULL,                                /* The parameter passed to the task - not used in this case. */
                1,                                   /* The priority assigned to the task. */
                NULL);                              /* The task handle is not required, so NULL is passed. */

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    for (;;);
}
/*
int main(void)
{
    prvSetupHardware();

    RunTasks();

    return 0;
}
*/
static void prvSetupHardware(void)
{
    BaseType_t xStatus;
    XScuGic_Config *pxGICConfig;

    /*
     * Ensure no interrupts execute while the scheduler is in an inconsistent
     * state.  Interrupts are automatically enabled when the scheduler is
     * started.
     */
    portDISABLE_INTERRUPTS();

    /* Obtain the configuration of the GIC. */
    pxGICConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);

    /*
     * Sanity check the FreeRTOSConfig.h settings are correct for the
     * hardware.
     */
    configASSERT(pxGICConfig);
    configASSERT(pxGICConfig->CpuBaseAddress == (configINTERRUPT_CONTROLLER_BASE_ADDRESS + configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET));
    configASSERT(pxGICConfig->DistBaseAddress == configINTERRUPT_CONTROLLER_BASE_ADDRESS);

    /* Install a default handler for each GIC interrupt. */
    xStatus = XScuGic_CfgInitialize(&xInterruptController, pxGICConfig, pxGICConfig->CpuBaseAddress);
    configASSERT(xStatus == XST_SUCCESS);

    /* Remove compiler warning if configASSERT() is not defined. */
    (void) xStatus;

    /*
     * The Xilinx projects use a BSP that do not allow the start up code to be
     * altered easily.  Therefore the vector table used by FreeRTOS is defined
     * in FreeRTOS_asm_vectors.S, which is part of this project. Switch to use
     * the FreeRTOS vector table.
     */
    vPortInstallFreeRTOSVectorTable();
}

void vApplicationMallocFailedHook(void)
{
    /*
     * Called if a call to pvPortMalloc() fails because there is insufficient
     * free memory available in the FreeRTOS heap.  pvPortMalloc() is called
     * internally by FreeRTOS API functions that create tasks, queues, software
     * timers, and semaphores.  The size of the FreeRTOS heap is set by the
     * configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h.
     */
    taskDISABLE_INTERRUPTS();
    for(;;);
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    /*
     * Run time stack overflow checking is performed if
     * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     * function is called if a stack overflow is detected.
     */
    taskDISABLE_INTERRUPTS();
    for(;;);
}

void vApplicationIdleHook(void)
{
    volatile size_t xFreeHeapSpace;

    /*
     * This is just a trivial example of an idle hook. It is called on each
     * cycle of the idle task. It must *NOT* attempt to block. In this case
     * the idle task just queries the amount of FreeRTOS heap that remains. See
     * the memory management section on the http://www.FreeRTOS.org web site for
     * memory management options. If there is a lot of heap memory free then the
     * configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
     * RAM.
     */
    xFreeHeapSpace = xPortGetFreeHeapSize();

    /* Remove compiler warning about xFreeHeapSpace being set but never used. */
    (void) xFreeHeapSpace;
}

void vAssertCalled(const char *pcFile, unsigned long ulLine)
{
    volatile unsigned long ul = 0;

    (void) pcFile;
    (void) ulLine;

    taskENTER_CRITICAL();
    {
        /*
         * Set ul to a non-zero value using the debugger to step out of this
         * function.
         */
        while (ul == 0)
        {
            portNOP();
        }
    }
    taskEXIT_CRITICAL();
}

void vApplicationTickHook(void)
{ }

void *memcpy(void *pvDest, const void *pvSource, size_t xBytes)
{
    /*
     * The compiler used during development seems to err unless these volatiles
     * are included at -O3 optimisation.
     */
    volatile unsigned char *pcDest = (volatile unsigned char *) pvDest, *pcSource = (volatile unsigned char *) pvSource;
    size_t x;

    /*
     * Extremely crude standard library implementations in lieu of having a C
     * library.
     */
    if (pvDest != pvSource) {
        for (x = 0; x < xBytes; x++) {
            pcDest[x] = pcSource[x];
        }
    }

    return pvDest;
}

void *memset(void *pvDest, int iValue, size_t xBytes)
{
    /*
     * The compiler used during development seems to err unless these volatiles
     * are included at -O3 optimisation.
     */
    volatile unsigned char * volatile pcDest = (volatile unsigned char * volatile) pvDest;
    volatile size_t x;

    /*
     * Extremely crude standard library implementations in lieu of having a C
     * library.
     */
    for (x = 0; x < xBytes; x++) {
        pcDest[x] = (unsigned char)iValue;
    }

    return pvDest;
}

int memcmp (const void *pvMem1, const void *pvMem2, size_t xBytes)
{
    const volatile unsigned char *pucMem1 = pvMem1, *pucMem2 = pvMem2;
    volatile size_t x;

    /*
     * Extremely crude standard library implementations in lieu of having a C
     * library.
     */
    for (x = 0; x < xBytes; x++) {
        if (pucMem1[x] != pucMem2[x]) {
            break;
        }
    }

    return xBytes - x;
}

void vInitialiseTimerForRunTimeStats(void)
{
    XScuWdt_Config *pxWatchDogInstance;
    uint32_t ulValue;
    const uint32_t ulMaxDivisor = 0xff, ulDivisorShift = 0x08;

    pxWatchDogInstance = XScuWdt_LookupConfig(XPAR_SCUWDT_0_DEVICE_ID);
    XScuWdt_CfgInitialize(&xWatchDogInstance, pxWatchDogInstance, pxWatchDogInstance->BaseAddr);

    ulValue = XScuWdt_GetControlReg(&xWatchDogInstance);
    ulValue |= ulMaxDivisor << ulDivisorShift;
    XScuWdt_SetControlReg(&xWatchDogInstance, ulValue);

    XScuWdt_LoadWdt(&xWatchDogInstance, UINT_MAX);
    XScuWdt_SetTimerMode(&xWatchDogInstance);
    XScuWdt_Start(&xWatchDogInstance);
}

