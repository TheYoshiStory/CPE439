#ifndef RANGEFINDER_H
#define RANGEFINDER_H

#include "gpio.h"

#define CLK_FREQ_HZ 100000000
#define SOUND_SPEED_MPS 343

#define ALPHA 50
#define THRESHOLD_SCALAR 50
#define THRESHOLD_BASE 250

#define SW_MASK 0x0000000F
#define LED_MASK 0x0000000F

void rangefinder_init();

uint32_t rangefinder_read();

#endif
