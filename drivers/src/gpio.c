#include "gpio.h"

// initializes GPIO driver
void gpio_init(XGpio *gpio)
{
	XGpio_Config *config;

	// look up configuration
	config = XGpio_LookupConfig(XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_CfgInitialize(gpio, config, config->BaseAddress);

	// initialize input channel
	XGpio_SetDataDirection(gpio, GPIO_INPUT, 0xFFFFFFFF);

	// initialize output channel
	XGpio_SetDataDirection(gpio, GPIO_OUTPUT, 0x00000000);
	XGpio_DiscreteWrite(gpio, GPIO_OUTPUT, 0x00000000);
}

// reads data from GPIO input channel
uint32_t gpio_read(XGpio *gpio)
{
	return(XGpio_DiscreteRead(gpio, GPIO_INPUT));
}

// writes data to GPIO output channel
void gpio_write(XGpio *gpio, uint32_t data)
{
	XGpio_DiscreteWrite(gpio, GPIO_OUTPUT, data);
}
