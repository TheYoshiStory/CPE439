#include "rangefinder.h"

static XGpio setting;
static XGpio rangefinder;
static uint32_t average[MAX_AVERAGE];
static int8_t index;

// initialize rangefinder
void rangefinder_init()
{
   uint32_t data;

   gpio_init(&setting, GPIO_DEVICE_0);
	gpio_init(&rangefinder, GPIO_DEVICE_2);

   for(index = 0; index < MAX_AVERAGE; index++)
   {
      average[index] = 0;
   }

   index = 0;
   
   data = gpio_read(&setting, GPIO_CHANNEL_1) & SW_MASK;
   gpio_write(&setting, GPIO_CHANNEL_2, data & LED_MASK);
}

// read rangefinder
uint32_t rangefinder_read()
{
	uint32_t data[2];
	uint32_t distance;
   int8_t count;

   data[0] = gpio_read(&setting, GPIO_CHANNEL_1) & SW_MASK;
	data[1] = gpio_read(&rangefinder, GPIO_CHANNEL_1);
	

   if(data[0] > MAX_AVERAGE)
   {
      data[0] = MAX_AVERAGE;
   }
   else if(data[0] < MIN_AVERAGE)
   {
      data[0] = MIN_AVERAGE;
   }

   index = (index + 1) % MAX_AVERAGE;
   average[index] = data[1];
   
   disatnce = 0;
   count = 0;

   while(count < data[0])
   {
      distance += average[(index - count) % MAX_AVERAGE]
      count++;
   }

   distance = distance * 1000 * SOUND_SPEED_MPS / CLK_FREQ_HZ / 2 / data[0];
   gpio_write(&setting, GPIO_CHANNEL_2, data[0] & LED_MASK);

	return(distance);
}
