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
	GPIO_MASK_SW 	= 0x0000000F,
	GPIO_MASK_LED	= 0x0000000F,
	GPIO_MASK_OD	= 0xFFFFFFFF,
	GPIO_MASK_RF	= 0xFFFFFFFF,
} gpio_mask_t;

typedef enum
{
	GPIO_SW0 = 0x00000001,
	GPIO_SW1 = 0x00000002,
	GPIO_SW2 = 0x00000004,
	GPIO_SW3 = 0x00000008,
} gpio_sw_t;

typedef enum
{
	GPIO_LED0 = 0x00000001,
	GPIO_LED1 = 0x00000002,
	GPIO_LED2 = 0x00000004,
	GPIO_LED3 = 0x00000008,
} gpio_led_t;

void gpio_init(XGpio *gpio, uint32_t device_id);

uint32_t gpio_read(XGpio *gpio, gpio_channel_t ch);

void gpio_write(XGpio *gpio, gpio_channel_t ch, uint32_t data);

#endif
