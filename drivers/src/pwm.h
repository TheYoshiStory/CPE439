#ifndef PWM_H
#define PWM_H

#include "xparameters.h"
#include "xil_io.h"

#define PWM_PERIOD_NS 20000000
#define CLK_PERIOD_NS 10

#define MAX_DUTY_CYCLE 100
#define MIN_DUTY_CYCLE 0

typedef enum
{
	PWM_CHANNEL_0 	= XPAR_AXI_TIMER_0_BASEADDR,
	PWM_CHANNEL_1	= XPAR_AXI_TIMER_1_BASEADDR,
	PWM_CHANNEL_2 	= XPAR_AXI_TIMER_2_BASEADDR,
	PWM_CHANNEL_3	= XPAR_AXI_TIMER_3_BASEADDR,
} pwm_channel_t;

void pwm_init();

void pwm_set(pwm_channel_t ch, uint8_t duty_cycle);

#endif
