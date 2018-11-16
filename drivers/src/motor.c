#include "motor.h"

// initialize motors
void motor_init()
{
	pwm_init();
	motor_control(0, 0);
}

// control motors
void motor_control(int8_t left, int8_t right)
{
	if(right > MAX_SPEED)
	{
		right = MAX_SPEED;
	}
	else if(right < (MAX_SPEED * -1))
	{
		right = MAX_SPEED * -1;
	}

	if(left > MAX_SPEED)
	{
		left = MAX_SPEED;
	}
	else if(left < (MAX_SPEED * -1))
	{
		left = MAX_SPEED * -1;
	}

	if(right > MIN_SPEED)
	{
		pwm_set(PWM_CHANNEL_0, MAX_DUTY_CYCLE - right);
		pwm_set(PWM_CHANNEL_1, MAX_DUTY_CYCLE);
	}
	else
	{
		pwm_set(PWM_CHANNEL_0, MAX_DUTY_CYCLE);
		pwm_set(PWM_CHANNEL_1, MAX_DUTY_CYCLE + right);
	}

	if(left > MIN_SPEED)
	{
		pwm_set(PWM_CHANNEL_2, MAX_DUTY_CYCLE);
		pwm_set(PWM_CHANNEL_3,  MAX_DUTY_CYCLE - left);
	}
	else
	{
		pwm_set(PWM_CHANNEL_2, MAX_DUTY_CYCLE + left);
		pwm_set(PWM_CHANNEL_3, MAX_DUTY_CYCLE);
	}
}
