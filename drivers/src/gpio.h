#ifndef GPIO_H
#define GPIO_H

#include "xparameters.h"
#include "xgpio.h"

#define SW_MASK		0x0000000F
#define LED_MASK 	0x0000000F

typedef enum
{
	GPIO_INPUT	= 1,
	GPIO_OUTPUT	= 2,
} gpio_channel_t;

typedef enum
{
	GPIO_SW0	= 0x0000001,
	GPIO_SW1	= 0x0000002,
	GPIO_SW2 	= 0x0000004,
	GPIO_SW3	= 0x0000008,
} gpio_sw_t;

typedef enum
{
	GPIO_LED0 	= 0x0000001,
	GPIO_LED1	= 0x0000002,
	GPIO_LED2 	= 0x0000004,
	GPIO_LED3 	= 0x0000008,
} gpio_led_t;


void gpio_init(XGpio *gpio);

uint32_t gpio_read(XGpio *gpio);

void gpio_write(XGpio *gpio, uint32_t data);

#endif
