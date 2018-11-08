#include "pwm.h"

// initializes PWM driver
void pwm_init()
{
	// setup control registers for PWM0
	Xil_Out32(PWM_CHANNEL_0, 0x206);
	Xil_Out32(PWM_CHANNEL_0 + 0x10, 0x606);
	Xil_Out32(PWM_CHANNEL_0 + 0x4, (PWM_PERIOD_NS / CLK_PERIOD_NS) - 2);

	// setup control registers for PWM1
	Xil_Out32(PWM_CHANNEL_1, 0x206);
	Xil_Out32(PWM_CHANNEL_1 + 0x10, 0x606);
	Xil_Out32(PWM_CHANNEL_1 + 0x4, (PWM_PERIOD_NS / CLK_PERIOD_NS) - 2);

	// setup control registers for PWM2
	Xil_Out32(PWM_CHANNEL_2, 0x206);
	Xil_Out32(PWM_CHANNEL_2 + 0x10, 0x606);
	Xil_Out32(PWM_CHANNEL_2 + 0x4, (PWM_PERIOD_NS / CLK_PERIOD_NS) - 2);

	// setup control registers for PWM3
	Xil_Out32(PWM_CHANNEL_3, 0x206);
	Xil_Out32(PWM_CHANNEL_3 + 0x10, 0x606);
	Xil_Out32(PWM_CHANNEL_3 + 0x4, (PWM_PERIOD_NS / CLK_PERIOD_NS) - 2);

	// initialize to 50% duty cycle
	Xil_Out32(PWM_CHANNEL_0 + 0x14, (PWM_DUTY_CYCLE_50 / CLK_PERIOD_NS) - 2);
	Xil_Out32(PWM_CHANNEL_1 + 0x14, (PWM_DUTY_CYCLE_50 / CLK_PERIOD_NS) - 2);
	Xil_Out32(PWM_CHANNEL_2 + 0x14, (PWM_DUTY_CYCLE_50 / CLK_PERIOD_NS) - 2);
	Xil_Out32(PWM_CHANNEL_3 + 0x14, (PWM_DUTY_CYCLE_50 / CLK_PERIOD_NS) - 2);
}

// sets the duty cycle of the given PWM channel
void pwm_set(pwm_addr_t pwm, pwm_duty_cycle_t duty_cycle)
{
	Xil_Out32(pwm + 0x14, (duty_cycle / CLK_PERIOD_NS) - 2);
}
