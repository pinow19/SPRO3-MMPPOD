/*
 * Testing_2.c
 *
 * Created: 08/11/2021 20.02.14
 * Author : danie
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "usart.h"


int main(void)
{
	uart_init();
	io_redirect();
	
	DDRB &= ~(1 << DDB0);
	DDRD |= (1 << DDD2);
	
	unsigned long int duration=0;
	float total_time, distance;
	
    /* Replace with your application code */
    while (1) 
    {
		PORTD &= (0<<PORTD2);
		_delay_us(2);
		PORTD |= (1<<PORTD2);
		_delay_us(10);
		PORTD &= (0<<PORTD2);
		
		while (duration == 0)
		{
			while (PINB & (1<<PINB0))
			{
				duration++;
			}
		}
		total_time = duration * 0.0625;
		distance = total_time * 0.034 / 2;
		printf("The result: %f\n", distance);
		duration = 0; 
		_delay_ms(60);
	}
}

