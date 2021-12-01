/*
 * Left wall follower algorithm.c
 *
 * Created: 24/11/2021 14.14.34
 * Author : Mihai Gruia
 */ 
#define F_CPU 16E6
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include "optical_sensor.h"
#include "usart.h"
#include "ultrasound_sensor.h"
#include "motor_board_controller.h"
#include <avr/delay.h>
#define PWM 200
#define DISTANCE 25
#define HIGH 1
#define LOW 0

int distance1=0,distance2=0,distance3=0; 
unsigned int Left_sensor=0,Front_sensor=0, Right_sensor=0;
void ultrasond_sensor_value(void);
void go_straight(void);
void turn_left(void);
void turn_right(void);
 
int main(void)
{
	uart_init();
	io_redirect();
	
	DDRD |= (1<<DDD3)|(1<<DDD5)|(1<<DDD7);
	DDRD &= (~(1<<DDD2))&(~(1<<DDD4))&(~(1<<DDD6));

    while (1) 
    {
		ultrasond_sensor_value();
		
		if( (Left_sensor<DISTANCE) && (Front_sensor<DISTANCE) && (Right_sensor<DISTANCE) )//turn around dead-end
		
		{
			turn_right();
			_delay_ms(2000);			
			
		}
		else if( (Left_sensor>DISTANCE) && (Front_sensor<DISTANCE) && (Right_sensor<DISTANCE) )
		{
			turn_left();
			_delay_ms(2000);
			
		}
		else if ( (Left_sensor<DISTANCE) && (Front_sensor<DISTANCE) && (Right_sensor>DISTANCE) )
		{
			turn_right();
			_delay_ms(2000);
		}
		else if ( (Left_sensor<DISTANCE) && (Front_sensor>DISTANCE) && (Right_sensor<DISTANCE) )
		{
			go_straight();
		}
		else if ( (Left_sensor>DISTANCE) && (Front_sensor>DISTANCE) && (Right_sensor<DISTANCE) )
		{
			go_straight();
		}
		else if ( (Left_sensor<DISTANCE) && (Front_sensor>DISTANCE) && (Right_sensor>DISTANCE) )
		{
			go_straight();
		}
		else go_straight();
		
    }
}
void ultrasond_sensor_value(void)
{
	Left_sensor=ultra_sound_sensor(1);
	printf("Sensor1: %d cm\n", Left_sensor);
	_delay_ms(1000);
	Front_sensor=ultra_sound_sensor(2);
	printf("Sensor2: %d cm\n", Front_sensor);
	_delay_ms(1000);
	Right_sensor=ultra_sound_sensor(3);
	printf("Sensor3: %d cm\n", Right_sensor);
	_delay_ms(1000);
}
void go_straight()
{
	M[0].state[0] = 1; M[0].state[1] = 0;
	M[1].state[0] = 1; M[1].state[1] = 0;
	motor_setup();
}


void turn_left()
{
	M[0].state[0] = 0; M[0].state[1] = 1;
	M[1].state[0] = 0; M[1].state[1]= 0;
	motor_setup();
}

void turn_right()
{
	M[0].state[0] = 0; M[0].state[1] = 0;
	M[1].state[0] = 0; M[1].state[1]= 1;
	motor_setup();
}