/*
 * Maze_solver.c
 *
 * Created: 12/12/2021 16.30.28
 * Author : danie
 */ 

#define F_CPU 16E6

#include <avr/io.h>
#include "samd.h"
#include <stdio.h>
#include "usart.h"
#include <util/delay.h>
#include "opto.h"

int flagg = 0;
int originalF = 0;
int count = 0;

int main(void)
{
	set_pins();
		
	uart_init();
	io_redirect();
	unsigned int FrontSensor, RightSensor, LeftSensor, DIS, _90_degrees, _10_degrees, _180_degrees, DIS_F, turning_1, turning_2, turning_3, turning_4;
	
	_90_degrees = 550; //ms
	_10_degrees = 110/2; //ms
	_180_degrees = 2000/2; //ms

	
	DIS = 20; // we give 5 cm wiggle room for the robot but we want it to be in the middle of the path
	DIS_F = 30;
	
    while (1) 
    {
		int PWM = 100;
		FrontSensor = ultrasound_sensor(U_FRONT);
		RightSensor = ultrasound_sensor(U_RIGHT);
		LeftSensor = ultrasound_sensor(U_LEFT);
		
		printf("Front: %d\n", FrontSensor);
		printf("Left: %d\n", LeftSensor);
		printf("Right: %d\n\n", RightSensor);	
		
		
		//There are seven cases our robot can come into contact with a obstacle and needs to avoid it
		
		//Case 1: When all three walls are within the defined range
		if (FrontSensor < DIS_F && RightSensor < DIS && LeftSensor < DIS)
		{
			printf("All three walls wrong\n");
			if (RightSensor > LeftSensor)
			{
							select_motor_direction(STOP, PWM);
							printf("STOPPPPP\n");
							_delay_ms(1000);
				select_motor_direction(RIGHT, PWM);
				_delay_ms(_180_degrees); //Time to turn the robot 180degrees
			} 
			else
			{
							select_motor_direction(STOP, PWM);
							printf("STOPPPPP\n");
							_delay_ms(1000);
				select_motor_direction(LEFT, PWM);
				_delay_ms(_180_degrees);
			}
		}
		
		//Case 2: When front is block
		else if (FrontSensor < DIS_F && RightSensor > DIS && LeftSensor > DIS)
		{
			printf("Front wall ahead\n");
			if (RightSensor > LeftSensor)
			{
							select_motor_direction(STOP, PWM);
							printf("STOPPPPP\n");
							_delay_ms(1000);
				select_motor_direction(RIGHT, PWM);
				_delay_ms(_90_degrees);
			} 
			else
			{
							select_motor_direction(STOP, PWM);
							printf("STOPPPPP\n");
							_delay_ms(1000);
				select_motor_direction(LEFT, PWM);
				_delay_ms(_90_degrees);
			}
		}
		/*
		//Case 3: Getting close to the left wall
		else if (FrontSensor > DIS_F && RightSensor > DIS && LeftSensor < DIS)
		{
						select_motor_direction(STOP);
						printf("STOPPPPP\n");
						_delay_ms(1000);
			printf("Left wall\n");
			select_motor_direction(RIGHT);
			_delay_ms(_10_degrees); =
			select_motor_direction(FORWARD);
		}
		
		//Case 4: Getting close to the right wall
		else if (FrontSensor > DIS_F && RightSensor < DIS && LeftSensor > DIS)
		{
						select_motor_direction(STOP);
						printf("STOPPPPP\n");
						_delay_ms(1000);
			printf("Right wall\n");
			select_motor_direction(LEFT);
			_delay_ms(_10_degrees);
			select_motor_direction(FORWARD);
		}
		*/
		//Case 5: When the front and left is blocked
		
		else if (FrontSensor < (DIS_F+20) && RightSensor > DIS && LeftSensor < DIS)
		{
			select_motor_direction(STOP, PWM);
			printf("STOPPPPPright\n");
			_delay_ms(1000);
			
			select_motor_direction(LEFT, 50); //MAKE SURE THE WIRING IS CORRECT
			printf("lol\n");
			opto_turn(4, OP_LEFT);
			printf("lol2\n");
			select_motor_direction(STOP, PWM);
			
		}
		//Case 6: When the front and right is blocked
		else if (FrontSensor < DIS_F && RightSensor < DIS && LeftSensor > DIS)
		{
			select_motor_direction(STOP, PWM);
			printf("STOPPPPPleft\n");
			_delay_ms(1000);
			
			select_motor_direction(RIGHT, 50); //MAKE SURE THE WIRING IS GOOD
			printf("lol\n");
			opto_turn(4, OP_RIGHT);
			printf("lol2\n");
			select_motor_direction(STOP, PWM);
		}
		
		//Case 7: If the sides are small but the front is free or all sides are free then move forward
		else
		{
			printf("Everything is ok!\n");
			select_motor_direction(FORWARD, PWM);
		}
	}
}

