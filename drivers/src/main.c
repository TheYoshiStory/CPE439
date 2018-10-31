#include "platform.h"
#include "gpio.h"
#include "pwm.h"

int main()
{
	XGpio gpio;
	uint32_t data;

	init_platform();

	gpio_init(&gpio);
	pwm_init();

	while(1)
	{
		data = gpio_read(&gpio) & SW_MASK;
		gpio_write(&gpio, data & LED_MASK);

		if(data & GPIO_SW0)
			pwm_set(PWM0, PWM_DUTY_CYCLE_50);
		else
			pwm_set(PWM0, PWM_DUTY_CYCLE_0);

		if(data & GPIO_SW1)
			pwm_set(PWM1, PWM_DUTY_CYCLE_50);
		else
			pwm_set(PWM1, PWM_DUTY_CYCLE_0);

		if(data & GPIO_SW2)
			pwm_set(PWM2, PWM_DUTY_CYCLE_50);
		else
			pwm_set(PWM2, PWM_DUTY_CYCLE_0);

		if(data & GPIO_SW3)
			pwm_set(PWM3, PWM_DUTY_CYCLE_50);
		else
			pwm_set(PWM3, PWM_DUTY_CYCLE_0);
	}

	cleanup_platform();

	return(0);
}
