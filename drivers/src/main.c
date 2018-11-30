#include <stdio.h>

#include "platform.h"
#include "sleep.h"
#include "motor.h"
#include "odometer.h"
#include "rangefinder.h"

int main()
{
	uint32_t data;

	init_platform();

	motor_init();
	odometer_init();
	rangefinder_init();

	while(1)
	{
		usleep(100000);
		data = rangefinder_read();
		printf("RANGEFINDER: %lumm\n\n\n\n", data);
	}

	cleanup_platform();

	return(0);
}
