#include <stdio.h>

#include "platform.h"
#include "sleep.h"
#include "motor.h"
#include "odometer.h"
#include "rangefinder.h"

int main()
{
	init_platform();

	motor_init();
	odometer_init();
	rangefinder_init();

	motor_control(MIN_SPEED, MIN_SPEED);

	while(1)
	{
		printf("RANGEFINDER: %lumm\n\n\n", rangefinder_read());
		usleep(100000);
	}

	cleanup_platform();

	return(0);
}
