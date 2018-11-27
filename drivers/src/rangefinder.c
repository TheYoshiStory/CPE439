#include "rangefinder.h"

static XGpio setting;
static XGpio rangefinder;
static uint32_t average[MAX_AVERAGE];
static uint8_t i;

// initialize rangefinder
void rangefinder_init()
{
	uint32_t data;

	gpio_init(&setting, GPIO_DEVICE_0);
	gpio_init(&rangefinder, GPIO_DEVICE_2);

	for(i = 0; i < MAX_AVERAGE; i++)
	{
		average[i] = 0;
	}

	i = 0;

	data = gpio_read(&setting, GPIO_CHANNEL_1) & SW_MASK;
	gpio_write(&setting, GPIO_CHANNEL_2, data & LED_MASK);
}

// read rangefinder
uint32_t rangefinder_read()
{
	uint32_t data[2];
	uint32_t distance;
	uint8_t count;

	data[0] = gpio_read(&setting, GPIO_CHANNEL_1) & SW_MASK;
	data[1] = gpio_read(&rangefinder, GPIO_CHANNEL_1);
	gpio_write(&setting, GPIO_CHANNEL_2, data[0] & LED_MASK);

	i = (i + 1) % MAX_AVERAGE;
	average[i] = data[1];

	distance = 0;
	count = 0;

	while(count < data[0])
	{
		distance += average[(i - count) % MAX_AVERAGE] * 1000 * SOUND_SPEED_MPS / CLK_FREQ_HZ / 2 / data[0];
		count++;
	}

	return(distance);
}
