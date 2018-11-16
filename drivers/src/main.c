#include <stdio.h>

#include "platform.h"
#include "sleep.h"
#include "motor.h"
#include "odometer.h"
#include "rangefinder.h"

int main()
{
	uint32_t data[2];

	init_platform();

	motor_init();
	odometer_init();
	rangefinder_init();

	motor_control(MIN_SPEED, MIN_SPEED);

	while(1)
	{
		data[0] = odometer_read();
		data[1] = rangefinder_read();

		printf("ODOMETER: %lumm\n", data[0]);
		printf("RANGEFINDER: %lumm\n\n", data[1]);

		sleep(1);
	}

	cleanup_platform();

	return(0);
}
