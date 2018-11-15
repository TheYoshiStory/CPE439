#include "odometer.h"

static XGpio odometer;

// initialize odometer
void odometer_init()
{
	gpio_init(&odometer, XPAR_AXI_GPIO_1_DEVICE_ID);
}

// read odometer
uint32_t odometer_read()
{
	uint32_t data[2];
	uint32_t distance;

	data[0] = gpio_read(&odometer, GPIO_CHANNEL_1) & GPIO_MASK_OD;
	data[1] = gpio_read(&odometer, GPIO_CHANNEL_2) & GPIO_MASK_OD;

	distance = (data[0] + data[1]) / 2;
	distance = (uint32_t)(distance * PI * WHEEL_DIAMETER / GEAR_RATIO / MAGNET_COUNT);

	return(distance);
}
