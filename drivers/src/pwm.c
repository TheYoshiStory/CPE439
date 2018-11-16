#include "pwm.h"

// initialize PWM driver
void pwm_init()
{
	Xil_Out32(PWM_CHANNEL_0, 0x206);
	Xil_Out32(PWM_CHANNEL_0 + 0x10, 0x606);
	Xil_Out32(PWM_CHANNEL_0 + 0x4, (PWM_PERIOD_NS / CLK_PERIOD_NS) - 2);
	Xil_Out32(PWM_CHANNEL_0 + 0x14, (PWM_PERIOD_NS / CLK_PERIOD_NS / 2) - 2);

	Xil_Out32(PWM_CHANNEL_1, 0x206);
	Xil_Out32(PWM_CHANNEL_1 + 0x10, 0x606);
	Xil_Out32(PWM_CHANNEL_1 + 0x4, (PWM_PERIOD_NS / CLK_PERIOD_NS) - 2);
	Xil_Out32(PWM_CHANNEL_1 + 0x14, (PWM_PERIOD_NS / CLK_PERIOD_NS / 2) - 2);

	Xil_Out32(PWM_CHANNEL_2, 0x206);
	Xil_Out32(PWM_CHANNEL_2 + 0x10, 0x606);
	Xil_Out32(PWM_CHANNEL_2 + 0x4, (PWM_PERIOD_NS / CLK_PERIOD_NS) - 2);
	Xil_Out32(PWM_CHANNEL_2 + 0x14, (PWM_PERIOD_NS / CLK_PERIOD_NS / 2) - 2);

	Xil_Out32(PWM_CHANNEL_3, 0x206);
	Xil_Out32(PWM_CHANNEL_3 + 0x10, 0x606);
	Xil_Out32(PWM_CHANNEL_3 + 0x4, (PWM_PERIOD_NS / CLK_PERIOD_NS) - 2);
	Xil_Out32(PWM_CHANNEL_3 + 0x14, (PWM_PERIOD_NS / CLK_PERIOD_NS / 2) - 2);
}

// set the duty cycle of the given PWM channel
void pwm_set(pwm_channel_t ch, uint8_t duty_cycle)
{
	if(duty_cycle >= MAX_DUTY_CYCLE)
	{
		Xil_Out32(ch + 0x14, (0 / CLK_PERIOD_NS) - 2);
	}
	else if(duty_cycle == MIN_DUTY_CYCLE)
	{
		Xil_Out32(ch + 0x14, (PWM_PERIOD_NS / CLK_PERIOD_NS) - 2);
	}
	else
	{
		Xil_Out32(ch + 0x14, (duty_cycle * PWM_PERIOD_NS / CLK_PERIOD_NS / 100) - 2);
	}
}
