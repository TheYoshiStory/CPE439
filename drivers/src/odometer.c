#include "odometer.h"

static XGpio odometer;

// initialize odometer
void odometer_init()
{
	gpio_init(&odometer, GPIO_DEVICE_1);
}

// read odometer
uint32_t odometer_read()
{
	uint32_t data[2];
	uint32_t distance;

	data[0] = gpio_read(&odometer, GPIO_CHANNEL_1);
	data[1] = gpio_read(&odometer, GPIO_CHANNEL_2);

	distance = (data[0] + data[1]) / 2;
	distance = (uint32_t)(distance * PI * WHEEL_DIAMETER / GEAR_RATIO / MAGNET_COUNT);

	return(distance);
}
