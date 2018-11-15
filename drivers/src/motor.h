#ifndef MOTOR_H
#define MOTOR_H

#include "pwm.h"

typedef enum
{
	MOTOR_SPEED_0 = PWM_DUTY_CYCLE_100,
	MOTOR_SPEED_5 = PWM_DUTY_CYCLE_95,
	MOTOR_SPEED_10 = PWM_DUTY_CYCLE_90,
	MOTOR_SPEED_15 = PWM_DUTY_CYCLE_85,
	MOTOR_SPEED_20 = PWM_DUTY_CYCLE_80,
	MOTOR_SPEED_25 = PWM_DUTY_CYCLE_75,
	MOTOR_SPEED_30 = PWM_DUTY_CYCLE_70,
	MOTOR_SPEED_35 = PWM_DUTY_CYCLE_65,
	MOTOR_SPEED_40 = PWM_DUTY_CYCLE_60,
	MOTOR_SPEED_45 = PWM_DUTY_CYCLE_55,
	MOTOR_SPEED_50 = PWM_DUTY_CYCLE_50,
	MOTOR_SPEED_55 = PWM_DUTY_CYCLE_45,
	MOTOR_SPEED_60 = PWM_DUTY_CYCLE_40,
	MOTOR_SPEED_65 = PWM_DUTY_CYCLE_35,
	MOTOR_SPEED_70 = PWM_DUTY_CYCLE_30,
	MOTOR_SPEED_75 = PWM_DUTY_CYCLE_25,
	MOTOR_SPEED_80 = PWM_DUTY_CYCLE_20,
	MOTOR_SPEED_85 = PWM_DUTY_CYCLE_15,
	MOTOR_SPEED_90 = PWM_DUTY_CYCLE_10,
	MOTOR_SPEED_95 = PWM_DUTY_CYCLE_5,
	MOTOR_SPEED_100 = PWM_DUTY_CYCLE_0,
} motor_speed_t;

void motor_init();

void motor_off();

void motor_forward(motor_speed_t speed);

void motor_backward(motor_speed_t speed);

void motor_right(motor_speed_t speed);

void motor_left(motor_speed_t speed);

#endif
