#include <stdio.h>

#include "platform.h"
#include "sleep.h"
#include "motor.h"
#include "odometer.h"
#include "gpio.h"

int main()
{
	XGpio rangefinder;

	init_platform();

	motor_init();
	odometer_init();
	gpio_init(&rangefinder, XPAR_AXI_GPIO_2_DEVICE_ID);

	while(1)
	{
		printf("RANGEFINDER: %lu\n", gpio_read(&rangefinder, GPIO_CHANNEL_1) & GPIO_MASK_RF);
		usleep(100);
	}

	cleanup_platform();

	return(0);
}
