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
int correcting = 0;

int main(void)
{
	set_pins();
		
	uart_init();
	io_redirect();
	unsigned int FrontSensor, RightSensor, LeftSensor, DIS, _90_degrees, _10_degrees, _180_degrees, DIS_F, turning_1, turning_2, turning_3, turning_4;
	
	_90_degrees = 550; //ms
	_10_degrees = 110/2; //ms
	_180_degrees = 2000/2; //ms

	
	DIS = 25; // we give 5 cm wiggle room for the robot but we want it to be in the middle of the path
	DIS_F = 20;
	
    while (1) 
    {
		int PWMR = 60;
		int PWML = 60;
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
							select_motor_direction(STOP, PWML, PWML);
							printf("STOPPPPP\n");
							_delay_ms(1000);
				select_motor_direction(RIGHT, PWML, PWMR);
				_delay_ms(_180_degrees); //Time to turn the robot 180degrees
			} 
			else
			{
							select_motor_direction(STOP, PWML, PWMR);
							printf("STOPPPPP\n");
							_delay_ms(1000);
				select_motor_direction(LEFT, PWML, PWMR);
				_delay_ms(_180_degrees);
			}
		}
		
		//Case 2: When front is block
		else if (FrontSensor < DIS_F && RightSensor > DIS && LeftSensor > DIS)
		{
			printf("Front wall ahead\n");
			if (RightSensor > LeftSensor)
			{
							select_motor_direction(STOP, PWML, PWMR);
							printf("STOPPPPP\n");
							_delay_ms(1000);
				select_motor_direction(RIGHT, PWML, PWMR);
				_delay_ms(_90_degrees);
			} 
			else
			{
							select_motor_direction(STOP, PWML, PWMR);
							printf("STOPPPPP\n");
							_delay_ms(1000);
				select_motor_direction(LEFT, PWML, PWMR);
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
		
		else if (FrontSensor < DIS_F && RightSensor > DIS && LeftSensor < DIS)
		{
			select_motor_direction(STOP, PWML, PWMR);
			printf("STOPPPPPright\n");
			_delay_ms(1000);
			
			
			printf("lol\n");
			for(int i = 0; i<3; i++){
				select_motor_direction(LEFT, 100, 100); //MAKE SURE THE WIRING IS GOOD
				opto_turn(3, OP_LEFT);
				select_motor_direction(STOP, PWML, PWMR);
				_delay_ms(200);
			}
			printf("lol2\n");
			select_motor_direction(STOP, PWML, PWMR);
			
		}
		//Case 6: When the front and right is blocked
		else if (FrontSensor < DIS_F && RightSensor < DIS && LeftSensor > DIS)
		{
			select_motor_direction(STOP, PWML, PWMR);
			printf("STOPPPPPleft\n");
			_delay_ms(1000);
			
			 
			printf("lol\n");
			for(int i = 0; i<3; i++){
			select_motor_direction(RIGHT, 100, 100); //MAKE SURE THE WIRING IS GOOD
			opto_turn(3, OP_RIGHT);
			select_motor_direction(STOP, PWML, PWMR);
			_delay_ms(200);
			}
			printf("lol2\n");
			select_motor_direction(STOP, PWML, PWMR);
		}
		
		//Case 7: If the sides are small but the front is free or all sides are free then move forward
		else
		{
			printf("Everything is ok!\n");
			select_motor_direction(FORWARD, PWMR, PWML);
			if (RightSensor>LeftSensor){
				while(RightSensor>LeftSensor){
				//correcting = RightSensor-LeftSensor;
				PWML = PWML+1;
				if (PWML>240) PWML = 250;
				select_motor_direction(FORWARD, PWMR, PWML);
				printf("Increasing left motor %d\n", PWML);
				RightSensor = ultrasound_sensor(U_RIGHT);
				LeftSensor = ultrasound_sensor(U_LEFT);
				}
				PWML = 50;
				PWMR = 50;
			}
			else if (LeftSensor>RightSensor){
				while (LeftSensor>RightSensor){
				//correcting = LeftSensor-RightSensor;
				PWMR = PWMR+1;
				if (PWMR>240) PWMR = 250;
				select_motor_direction(FORWARD, PWMR, PWML);
				printf("Increasing right motor %d\n", PWMR);
				RightSensor = ultrasound_sensor(U_RIGHT);
				LeftSensor = ultrasound_sensor(U_LEFT);
				}
				PWML = 50;
				PWMR = 50;
			}
		}
	}
}

