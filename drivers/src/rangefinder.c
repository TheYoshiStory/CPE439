#include "rangefinder.h"

static XGpio rangefinder;

// initialize rangefinder
void rangefinder_init()
{
	gpio_init(&rangefinder, GPIO_DEVICE_2);
}

// read rangefinder
uint32_t rangefinder_read()
{
	uint32_t data;
	uint32_t distance;

	data = gpio_read(&rangefinder, GPIO_CHANNEL_1);
	distance = data * 1000 * SOUND_SPEED_MPS / CLK_FREQ_HZ / 2;

	return(distance);
}
