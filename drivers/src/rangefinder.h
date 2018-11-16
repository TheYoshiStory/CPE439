#ifndef RANGEFINDER_H
#define RANGEFINDER_H

#include "gpio.h"

#define CLK_FREQ_HZ		100000000
#define SOUND_SPEED_MPS	343

void rangefinder_init();

uint32_t rangefinder_read();

#endif
