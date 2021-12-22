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
#include <math.h>


int main(void)
{
	set_pins();
	uart_init();
	io_redirect();
	
	unsigned int FrontSensor, RightSensor, LeftSensor, DIS_F = 25, unstuck = 0, batttery_cycle = 0; //Left 8-12 cm and Front around 22cm
	unsigned char PWM;
	double voltage_level, temperature_sensor;
	
	FrontSensor = ultrasound_sensor(U_FRONT);
	RightSensor = ultrasound_sensor(U_RIGHT);
	LeftSensor = ultrasound_sensor(U_LEFT);
	
	voltage_level = analog_sensor(VOLTAGE_SENSOR);
	PWM = 6.25*pow(voltage_level,2)-111.25*voltage_level+551.25;

	while (1)
	{
		if (batttery_cycle == 10000)
		{
			voltage_level = analog_sensor(VOLTAGE_SENSOR);
			PWM = 6.25*pow(voltage_level,2)-111.25*voltage_level+551.25;
			batttery_cycle = 0;
			temperature_sensor = analog_sensor(TEMP_SENSOR)*100;
			if (temperature_sensor > 55) //maximum operating Celsius degree
			{
				while (1)
				{
					//infinite loop  ´danie_szanka_the<-cat<-destroyer<-6<-<9_dog<-_lover    
				}
			}
		}
		
		batttery_cycle++;
		printf("PWM: %d Voltage: %f\n", PWM, voltage_level);
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
			unstuck = 0;
		}
		else if ((LeftSensor > 20) && (FrontSensor >= DIS_F))
		{
			printf("\nMissing left wall - need to close in\n");
			unstuck++;
			select_motor_direction(LEFT, PWM);
			_delay_ms(200);
			select_motor_direction(FORWARD, PWM);
			_delay_ms(200);
			if (unstuck > 5)
			{
				select_motor_direction(LEFT, PWM);
				_delay_ms(200);
				select_motor_direction(FORWARD, PWM);
			}
			
		}
		else if ((LeftSensor < 10) && (FrontSensor >= DIS_F))
		{
			unstuck = 0;
			printf("\nClose to left wall - need to turn right\n");
			select_motor_direction(RIGHT, PWM);
			_delay_ms(200);
			select_motor_direction(FORWARD, PWM);
		}
		else if (FrontSensor < DIS_F)
		{
			unstuck = 0;
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

