#include <stdio.h>
#include "platform.h"
#include "gpio.h"
#include "pwm.h"

int main()
{
	XGpio debug;
	XGpio tach;
	uint32_t data[3];

	init_platform();

	printf("hello world!\n");

	gpio_init(&debug, XPAR_AXI_GPIO_0_DEVICE_ID);
	gpio_init(&tach, XPAR_AXI_GPIO_1_DEVICE_ID);
	pwm_init();

	while(1)
	{
		data[0] = gpio_read(&debug, GPIO_CHANNEL_1) & GPIO_MASK_SW;
		data[1] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
		data[2] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
		gpio_write(&debug, GPIO_CHANNEL_2, data[0] & GPIO_MASK_LED);

		printf("TACH0 = %lu     |     TACH1 = %lu\n", data[1], data[2]);

		if(data[0] & GPIO_SW0)
			pwm_set(PWM0, PWM_DUTY_CYCLE_75);
		else
			pwm_set(PWM0, PWM_DUTY_CYCLE_100);

		if(data[0] & GPIO_SW1)
			pwm_set(PWM1, PWM_DUTY_CYCLE_75);
		else
			pwm_set(PWM1, PWM_DUTY_CYCLE_100);

		if(data[0] & GPIO_SW2)
			pwm_set(PWM2, PWM_DUTY_CYCLE_75);
		else
			pwm_set(PWM2, PWM_DUTY_CYCLE_100);

		if(data[0] & GPIO_SW3)
			pwm_set(PWM3, PWM_DUTY_CYCLE_75);
		else
			pwm_set(PWM3, PWM_DUTY_CYCLE_100);
	}

	cleanup_platform();

	return(0);
}
