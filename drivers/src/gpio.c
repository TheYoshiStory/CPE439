#include "gpio.h"

// initialize GPIO driver
void gpio_init(XGpio *gpio, uint32_t device_id)
{
	XGpio_Config *config;

	config = XGpio_LookupConfig(device_id);
	XGpio_CfgInitialize(gpio, config, config->BaseAddress);

	switch(device_id)
	{
		case XPAR_AXI_GPIO_0_DEVICE_ID:
			XGpio_SetDataDirection(gpio, GPIO_CHANNEL_1, 0xFFFFFFFF);
			XGpio_SetDataDirection(gpio, GPIO_CHANNEL_2, 0x00000000);
			XGpio_DiscreteWrite(gpio, GPIO_CHANNEL_2, 0x00000000);
			break;

		case XPAR_AXI_GPIO_1_DEVICE_ID:
			XGpio_SetDataDirection(gpio, GPIO_CHANNEL_1, 0xFFFFFFFF);
			XGpio_SetDataDirection(gpio, GPIO_CHANNEL_2, 0xFFFFFFFF);
			break;

		case XPAR_AXI_GPIO_2_DEVICE_ID:
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
