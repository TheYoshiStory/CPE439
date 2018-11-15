#ifndef ODOMETER_H
#define ODOMETER_H

#include "gpio.h"

#define PI				3.14159265358979323846
#define WHEEL_DIAMETER	65
#define GEAR_RATIO		48
#define	MAGNET_COUNT	4

void odometer_init();

uint32_t odometer_read();

#endif
