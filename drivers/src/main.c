#include <stdio.h>

#include "gpio.h"
#include "motor.h"
#include "platform.h"

int main()
{
	XGpio gpio;
	uint32_t data;

	init_platform();

	gpio_init(&gpio, XPAR_AXI_GPIO_0_DEVICE_ID);
	motor_init();

	motor_control(MOTOR_FORWARD, MOTOR_SPEED_25, 1000);

	while(1)
	{
		data = gpio_read(&gpio, GPIO_CHANNEL_1) & GPIO_MASK_SW;
		gpio_write(&gpio, GPIO_CHANNEL_2, data & GPIO_MASK_LED);
	}

	cleanup_platform();

	return(0);
}
