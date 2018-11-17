#ifndef RANGEFINDER_H
#define RANGEFINDER_H

#include "gpio.h"

#define CLK_FREQ_HZ 100000000
#define SOUND_SPEED_MPS 343

#define MAX_AVERAGE 15
#define MIN_AVERAGE 1

#define SW_MASK 0x0000000F
#define LED_MASK 0x0000000F

void rangefinder_init();

uint32_t rangefinder_read();

#endif
