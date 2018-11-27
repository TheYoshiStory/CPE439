#ifndef MOTOR_H
#define MOTOR_H

#include "pwm.h"

#define MAX_SPEED 100
#define MIN_SPEED 0

void motor_init();

void motor_control(int8_t left, int8_t right);

#endif
