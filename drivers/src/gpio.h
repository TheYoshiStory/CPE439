#ifndef GPIO_H
#define GPIO_H

#include "xparameters.h"
#include "xgpio.h"

typedef enum
{
	GPIO_CHANNEL_1 = 1,
	GPIO_CHANNEL_2 = 2,
} gpio_channel_t;

typedef enum
{
	GPIO_DEVICE_0 = XPAR_AXI_GPIO_0_DEVICE_ID,
	GPIO_DEVICE_1 = XPAR_AXI_GPIO_1_DEVICE_ID,
	GPIO_DEVICE_2 = XPAR_AXI_GPIO_2_DEVICE_ID,
} gpio_device_t;

void gpio_init(XGpio *gpio, gpio_device_t id);

uint32_t gpio_read(XGpio *gpio, gpio_channel_t ch);

void gpio_write(XGpio *gpio, gpio_channel_t ch, uint32_t data);

#endif
