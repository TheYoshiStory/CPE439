#include <stdio.h>
#include "platform.h"
#include "gpio.h"
#include "pwm.h"
void motorsOff(void);
void motorsForward(void);
void motorsBackward(void);
void motorsLeft(void);
void motorsRight(void);
void motorController(char direction, uint32_t distance);

XGpio debug;
XGpio tach;

int main()
{

	uint32_t data,distance;
	init_platform();

	gpio_init(&debug, XPAR_AXI_GPIO_0_DEVICE_ID);
	gpio_init(&tach, XPAR_AXI_GPIO_1_DEVICE_ID);
	pwm_init();
	data = gpio_read(&debug, GPIO_CHANNEL_1) & GPIO_MASK_SW;
	gpio_write(&debug, GPIO_CHANNEL_2, data&GPIO_MASK_LED);
	distance = 1000;
	if(data & GPIO_SW0){
		distance += 1000;
	}
	if(data & GPIO_SW1){
		distance += 1000;
	}
	if(data & GPIO_SW2){
		distance += 1000;
	}
	if(data & GPIO_SW3){
		distance += 1000;
	}

	motorController('F', distance);
	//motorController('B', 2500);
	while(1)
	{
	}

	cleanup_platform();

	return(0);
}

void motorController(char direction, uint32_t distance){
	uint32_t tachCount[2];
	uint32_t count[2];
	tachCount[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
	tachCount[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
	uint32_t averageTach = (tachCount[0]+tachCount[1])/2;
	count[0] = distance + averageTach;
	//count[1] = distance + tachCount[1];
	printf("%lu:",tachCount[0]);
	printf("%lu\n",tachCount[1]);
	printf("%lu\n",count[0]);
	//printf("%lu\n",count[1]);

	if(direction == 'F'){
		motorsForward();
		/*
		while(count[0] > tachCount[0] && count[1] > tachCount[1]){
			tachCount[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
			tachCount[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
		}
		*/
		while(count[0] > averageTach){
			tachCount[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
			tachCount[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
			averageTach = (tachCount[0]+tachCount[1])/2;
		}
		motorsOff();
	}
	else if(direction == 'B'){
		motorsBackward();
		/*
		while(count[0] > tachCount[0] && count[1] > tachCount[1]){
			tachCount[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
			tachCount[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
		}
		*/
		while(count[0] > averageTach){
			tachCount[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
			tachCount[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
			averageTach = (tachCount[0]+tachCount[1])/2;
		}
		motorsOff();
	}
	else if(direction == 'L'){
		motorsLeft();
		/*
		while(count[0] > tachCount[0] && count[1] > tachCount[1]){
			tachCount[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
			tachCount[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
		}
		*/
		while(count[0] > averageTach){
			tachCount[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
			tachCount[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
			averageTach = (tachCount[0]+tachCount[1])/2;
		}
		motorsOff();
	}
	else if(direction == 'R'){
		motorsRight();
		/*
		while(count[0] > tachCount[0] && count[1] > tachCount[1]){
			tachCount[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
			tachCount[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
		}
		*/
		while(count[0] > averageTach){
			tachCount[0] = gpio_read(&tach, GPIO_CHANNEL_1) & GPIO_MASK_TACH;
			tachCount[1] = gpio_read(&tach, GPIO_CHANNEL_2) & GPIO_MASK_TACH;
			averageTach = (tachCount[0]+tachCount[1])/2;
		}
		motorsOff();
	}
	else{
		motorsOff();
	}
}

void motorsOff(void){
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_3, PWM_DUTY_CYCLE_100);
}

void motorsForward(void){
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_75);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_3, (PWM_PERIOD_NS * 74) / 100);
}

void motorsBackward(void){
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_80);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_80);
	pwm_set(PWM_CHANNEL_3, PWM_DUTY_CYCLE_100);
}

void motorsRight(void){
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_80);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_3, PWM_DUTY_CYCLE_80);
}

void motorsLeft(void){
	pwm_set(PWM_CHANNEL_0, PWM_DUTY_CYCLE_80);
	pwm_set(PWM_CHANNEL_1, PWM_DUTY_CYCLE_100);
	pwm_set(PWM_CHANNEL_2, PWM_DUTY_CYCLE_80);
	pwm_set(PWM_CHANNEL_3, PWM_DUTY_CYCLE_100);
}
