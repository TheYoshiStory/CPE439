#include "gpio.h"

// initialize GPIO driver
void gpio_init(XGpio *gpio, gpio_device_t id)
{
	XGpio_Config *config;

	config = XGpio_LookupConfig(id);
	XGpio_CfgInitialize(gpio, config, config->BaseAddress);

	switch(id)
	{
		case GPIO_DEVICE_O:
			XGpio_SetDataDirection(gpio, GPIO_CHANNEL_1, 0xFFFFFFFF);
			XGpio_SetDataDirection(gpio, GPIO_CHANNEL_2, 0x00000000);
			XGpio_DiscreteWrite(gpio, GPIO_CHANNEL_2, 0x00000000);
			break;

		case GPIO_DEVICE_1:
			XGpio_SetDataDirection(gpio, GPIO_CHANNEL_1, 0xFFFFFFFF);
			XGpio_SetDataDirection(gpio, GPIO_CHANNEL_2, 0xFFFFFFFF);
			break;

		case GPIO_DEVICE_2:
			XGpio_SetDataDirection(gpio, GPIO_CHANNEL_1, 0xFFFFFFFF);
			break;
	}
}

// read data from GPIO channel
uint32_t gpio_read(XGpio *gpio, gpio_channel_t ch)
{
	return(XGpio_DiscreteRead(gpio, ch));
}

// write data to GPIO channel
void gpio_write(XGpio *gpio, gpio_channel_t ch, uint32_t data)
{
	XGpio_DiscreteWrite(gpio, ch, data);
}
