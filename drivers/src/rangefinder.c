#include "rangefinder.h"

static XGpio setting;
static XGpio rangefinder;

// initialize rangefinder
void rangefinder_init()
{
	gpio_init(&setting, GPIO_DEVICE_0);
	gpio_init(&rangefinder, GPIO_DEVICE_2);
}

//output to LEDs
void LED_distance(uint32_t distance){
	static uint32_t LEDs = 0;
	static uint32_t switches = 0;
	static uint32_t threshold = 0;

	switches = gpio_read(&setting, GPIO_CHANNEL_1) & SW_MASK;
	threshold = 10 * switches + MAX_DISTANCE;


	if(distance > threshold){
		LEDs = 0;
		gpio_write(&setting, GPIO_CHANNEL_2, LEDs);
	}
	else if(threshold * 9/10 <= distance && distance < threshold){
		LEDs = (LEDs & 0x1) ^ 0x1;
		gpio_write(&setting, GPIO_CHANNEL_2, LEDs);
	}
	else if(threshold * 8/10 <= distance && distance < threshold * 9/10){
		LEDs = 0x1;
		gpio_write(&setting, GPIO_CHANNEL_2, LEDs);
	}
	else if(threshold * 7/10 <= distance && distance < threshold * 8/10){
		LEDs = (((LEDs | 0x1) & 0x3) ^ 0x2);
		gpio_write(&setting, GPIO_CHANNEL_2, LEDs);
	}
	else if(threshold * 6/10 <= distance && distance < threshold * 7/10){
		LEDs = 0x3;
		gpio_write(&setting, GPIO_CHANNEL_2, LEDs);
	}
	else if(threshold * 5/10 <= distance && distance < threshold * 6/10){
		LEDs = (((LEDs | 0x3) & 0x7) ^ 0x4);
		gpio_write(&setting, GPIO_CHANNEL_2, LEDs);
	}
	else if(threshold * 4/10 <= distance && distance < threshold * 5/10){
		LEDs = 0x7;
		gpio_write(&setting, GPIO_CHANNEL_2, LEDs);
	}
	else if(threshold * 3/10 <= distance && distance <threshold * 4/10){
		LEDs = ((LEDs | 0x7) ^ 0x8);
		gpio_write(&setting, GPIO_CHANNEL_2, LEDs);
	}
	else if(threshold * 2/10 <= distance && distance < threshold * 3/10){
		LEDs = 0xF;
		gpio_write(&setting, GPIO_CHANNEL_2, LEDs);
	}
	else if(distance < threshold * 2/10){
		LEDs = LEDs ^ 0xF;
		gpio_write(&setting, GPIO_CHANNEL_2, LEDs);
	}
}

// read rangefinder
uint32_t rangefinder_read()
{
	static uint8_t i = 0;
	static uint32_t distance;
	uint32_t output;
	uint32_t data[2];

	data[0] = gpio_read(&setting, GPIO_CHANNEL_1) & SW_MASK;
	data[1] = gpio_read(&rangefinder, GPIO_CHANNEL_1);

	if(i == 0){
		distance = data[1];
		i = 1;
	}
	else{
		distance = (data[1] * 50 + distance * 50)/100;
	}
	output = distance * SOUND_SPEED_MPS / (CLK_FREQ_HZ / 1000) / 2;
	LED_distance(output);
	return (output);
}


