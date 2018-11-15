#include <stdio.h>

#include "platform.h"
#include "sleep.h"
#include "odometer.h"
#include "motor.h"

int main()
{
	init_platform();

	odometer_init();
	motor_init();

	while(1)
	{
		printf("ODOMETER: %lu\n", odometer_read());
		usleep(100);
	}

	cleanup_platform();

	return(0);
}
