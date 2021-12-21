/*
 * Maze_solver.c
 *
 * Created: 12/12/2021 16.30.28
 * Author : Daniel Szanka
 */ 

#define F_CPU 16E6

#include <avr/io.h>
#include "samd.h"
#include <stdio.h>
#include "usart.h"
#include <util/delay.h>


int main(void)
{
	set_pins();
	uart_init();
	io_redirect();
	
	unsigned int FrontSensor, RightSensor, LeftSensor, DIS_F = 25; //Left 8-12 cm and Front around 22cm
	unsigned char PWM = 65;
	
	FrontSensor = ultrasound_sensor(U_FRONT);
	RightSensor = ultrasound_sensor(U_RIGHT);
	LeftSensor = ultrasound_sensor(U_LEFT);
	
	while (1)
	{
		FrontSensor = ultrasound_sensor(U_FRONT);
		RightSensor = ultrasound_sensor(U_RIGHT);
		LeftSensor = ultrasound_sensor(U_LEFT);	
		
		printf("Front: %d cm\n", FrontSensor);
		printf("Left: %d cm\n", LeftSensor);
		printf("Right: %d cm\n", RightSensor);
		
		if ((LeftSensor >= 10) && (LeftSensor <= 20) && (FrontSensor >= DIS_F))
		{
			printf("\nGoing forward\n");
			select_motor_direction(FORWARD, PWM);
		}
		else if ((LeftSensor > 20) && (FrontSensor >= DIS_F))
		{
			printf("\nMissing left wall - need to close in\n");
			select_motor_direction(LEFT, PWM);
			_delay_ms(200);
			select_motor_direction(FORWARD, PWM);
		}
		else if ((LeftSensor < 10) && (FrontSensor >= DIS_F))
		{
			printf("\nClose to left wall - need to turn right\n");
			select_motor_direction(RIGHT, PWM);
			_delay_ms(200);
			select_motor_direction(FORWARD, PWM);
		}
		else if (FrontSensor < DIS_F)
		{
			printf("\nFront wall blocked\n");
			select_motor_direction(STOP, 0);
			_delay_ms(1000);
			if (RightSensor > 22)
			{
				printf("\nEnough space on the right\n");
				optocoupler_slids(O_LEFT);
				while (slid_nr <= 8)
				{
					select_motor_direction(RIGHT, PWM);
				}
				reset_slid_nr_and_stop_interrupt();
			} 
			else
			{
				printf("\nBlocked everywhere, turn 180 degrees\n");
				optocoupler_slids(O_LEFT);
				while (slid_nr <= 18)
				{
					select_motor_direction(LEFT, PWM);
				}
			}
		}
	}
}

