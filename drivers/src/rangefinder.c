#include "rangefinder.h"

static XGpio setting;
static XGpio rangefinder;
static uint32_t distance;

// initialize rangefinder
void rangefinder_init()
{
	gpio_init(&setting, GPIO_DEVICE_0);
	gpio_init(&rangefinder, GPIO_DEVICE_2);
	distance = 0;
}

// read rangefinder
uint32_t rangefinder_read()
{
	static uint8_t i = 0;
	static uint32_t average = 0;
	uint32_t data[2];

	data[0] = gpio_read(&setting, GPIO_CHANNEL_1) & SW_MASK;
	data[1] = gpio_read(&rangefinder, GPIO_CHANNEL_1);
	gpio_write(&setting, GPIO_CHANNEL_2, data[0] & LED_MASK);
	data[0] = (data[0] + 1) * AVG_SCALE;

	if(i < data[0])
	{
		average += data[1] / data[0];
		i++;
	}
	else
	{
		distance = average * SOUND_SPEED_MPS / (CLK_FREQ_HZ / 1000) / 2;
		average = 0;
		i = 0;
	}

	return(distance);
}
