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
	static uint32_t filter;
	uint32_t distance;
	uint32_t threshold;
	uint32_t data[3];

	data[0] = gpio_read(&setting, GPIO_CHANNEL_1) & SW_MASK;
	data[1] = gpio_read(&setting, GPIO_CHANNEL_2) & LED_MASK;
	data[2] = gpio_read(&rangefinder, GPIO_CHANNEL_1);

	filter = (ALPHA * filter + (100 - ALPHA) * data[2]) / 100;
	distance = filter * SOUND_SPEED_MPS / (CLK_FREQ_HZ / 1000) / 2;
	threshold = data[0] * THRESHOLD_SCALAR + THRESHOLD_BASE;

	if(distance > threshold)
	{
		data[1] = 0x0;
	}
	else if(distance > threshold * 8 / 9)
	{
		data[1] = (data[1] & 0x1) ^ 0x01;
	}
	else if(distance > threshold * 7 / 9)
	{
		data[1] = 0x1;
	}
	else if(distance > threshold * 6 / 9)
	{
		data[1] =  ((data[1] & 0x3) | 0x1) ^ 0x02;
	}
	else if(distance > threshold * 5 / 9)
	{
		data[1] = 0x3;
	}
	else if(distance > threshold * 4 / 9)
	{
		data[1] =  ((data[1] & 0x7) | 0x3) ^ 0x04;
	}
	else if(distance > threshold * 3 / 9)
	{
		data[1] = 0x7;
	}
	else if(distance > threshold * 2 / 9)
	{
		data[1] =  ((data[1] & 0xF) | 0x7) ^ 0x08;
	}
	else if(distance > threshold * 1 / 9)
	{
		data[1] = 0xF;
	}
	else
	{
		if(data[1])
		{
			data[1] = 0x0;
		}
		else
		{
			data[1] = 0xF;
		}
	}

	gpio_write(&setting, GPIO_CHANNEL_2, data[1] & LED_MASK);

	return(distance);
}


