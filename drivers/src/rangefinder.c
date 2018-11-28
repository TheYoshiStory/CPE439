#include "rangefinder.h"

static XGpio setting;
static XGpio rangefinder;

// initialize rangefinder
void rangefinder_init()
{
	gpio_init(&setting, GPIO_DEVICE_0);
	gpio_init(&rangefinder, GPIO_DEVICE_2);
}

// read rangefinder
uint32_t rangefinder_read()
{
	uint32_t data[2];
	uint32_t distance;
	uint8_t i;

	data[0] = gpio_read(&setting, GPIO_CHANNEL_1) & SW_MASK;
	gpio_write(&setting, GPIO_CHANNEL_2, data[0] & LED_MASK);

	data[0] = (data[0] + 1) * 4;
	distance = 0;

	for(i = 0; i < data[0]; i++)
	{
		data[1] = gpio_read(&rangefinder, GPIO_CHANNEL_1);
		distance += data[1] * SOUND_SPEED_MPS / (CLK_FREQ_HZ / 1000) / 2;
		usleep(10000);
	}

	distance /= data[0];

	return(distance);
}
