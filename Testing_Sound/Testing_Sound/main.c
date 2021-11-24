/*
 * Testing_Sound.c
 *
 * Created: 24/11/2021 17.18.41
 * Author : danie
 */ 

#define F_CPU 16E6
#include <avr/io.h>
#include <stdio.h>
#include "ultrasound_sensor.h"
#include <util/delay.h>
#include "usart.h"


int main(void)
{
	uart_init();
	io_redirect();
	unsigned int Sensor1, Sensor2, Sensor3;
    /* Replace with your application code */
    while (1) 
    {
		Sensor1=ultra_sound_sensor(1);
		printf("Sensor1: %d cm\n", Sensor1);
		_delay_ms(1000);
		Sensor2=ultra_sound_sensor(2);
		printf("Sensor2: %d cm\n", Sensor2);
		_delay_ms(1000);
		Sensor3=ultra_sound_sensor(3);
		printf("Sensor3: %d cm\n", Sensor3);
		_delay_ms(1000);
    }
}

