#include "motor.h"

// initialize motors
void motor_init()
{
	pwm_init();
	motor_off();
}

// stop motors
void motor_off()
{
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_3, PWM_DUTY_CYCLE_100);
}

// run motors forward
void motor_forward(motor_speed_t speed)
{
	pwm_set(PWM_CHANNEL_0, speed);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_3, speed);
}

// run motors backward
void motor_backward(motor_speed_t speed)
{
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_1, speed);
	pwm_set(PWM_CHANNEL_2, speed);
	pwm_set(PWM_CHANNEL_3, PWM_DUTY_CYCLE_100);
}

// turn motors right
void motor_right(motor_speed_t speed)
{
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_1, speed);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_3, speed);
}

// turn motors left
void motor_left(motor_speed_t speed)
{
	pwm_set(PWM_CHANNEL_0, speed);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_2, speed);
	pwm_set(PWM_CHANNEL_3, PWM_DUTY_CYCLE_100);
}
