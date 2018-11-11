#include "motor.h"

static XGpio tach;

// stop motors
static void motor_off()
{
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_3, PWM_DUTY_CYCLE_100);
}

// run motors forward
static void motor_forward(motor_speed_t speed)
{
	pwm_set(PWM_CHANNEL_0, speed);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_3, speed);
}

// run motors backward
static void motor_backward(motor_speed_t speed)
{
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_1, speed);
	pwm_set(PWM_CHANNEL_2, speed);
	pwm_set(PWM_CHANNEL_3, PWM_DUTY_CYCLE_100);
}

// turn motors right
static void motor_right(motor_speed_t speed)
{
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_1, speed);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_3, speed);
}

// turn motors left
static void motor_left(motor_speed_t speed)
{
	pwm_set(PWM_CHANNEL_0, speed);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_2, speed);
	pwm_set(PWM_CHANNEL_3, PWM_DUTY_CYCLE_100);
}

// initialize motors
void motor_init()
{
	gpio_init(&tach, XPAR_AXI_GPIO_1_DEVICE_ID);
	pwm_init();
	motor_off();
}

void motor_control(motor_direction_t direction, motor_speed_t speed, uint32_t distance)
{
	uint32_t data[2];
	uint32_t average;
	uint32_t count;

	data[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
	data[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
	average = (data[0] + data[1]) / 2;
	count = average + distance;

	switch(direction)
	{
		case MOTOR_FORWARD:
			motor_forward(speed);
			while(average < count)
			{
				data[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
				data[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
				average = (data[0] + data[1]) / 2;
			}
			break;

		case MOTOR_BACKWARD:
			motor_backward(speed);
			while(average < count)
			{
				data[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
				data[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
				average = (data[0] + data[1]) / 2;
			}
			break;

		case MOTOR_RIGHT:
			motor_right(speed);
			while(average < count)
			{
				data[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
				data[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
				average = (data[0] + data[1]) / 2;
			}
			break;

		case MOTOR_LEFT:
			motor_left(speed);
			while(average < count)
			{
				data[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
				data[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
				average = (data[0] + data[1]) / 2;
			}
			break;
	}

	motor_off();
}
