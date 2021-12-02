/*
 * ulrasonic.c
 *
 * Created: 26-10-2021 21:22:43
 * Author : ttoft
 */ 
#define F_CPU 16E6
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <time.h>
#include "usart.h"

void opena1(int);

int main(void)
{
	unsigned int time1;
	unsigned int time2;
    uart_init();
	io_redirect();
	DDRD |= (1<<PIND3) | (1<<PIND6);
	DDRD &= ~(1<<PIND2);
    while (1) 
    {
		PORTD &= ~(1<<PIND3);
		_delay_us(20);
		PORTD |= (1<<PIND3);
		_delay_us(10);
		PORTD &= ~(1<<PIND3);
		TCCR1B |= (1<<CS11);
		while(!(PIND & (1<<PIND2)))
		{
			time1 = TCNT1;
		}
		while ((PIND & (1<<PIND2)))
		{
			time2 = TCNT1;
		}
		TCCR1B &= !(1<<CS11);
		float timeprint = (time2-time1)*0.008575;
		printf("%f\n", timeprint);
		_delay_ms(2000);
		TCNT1 = 0;
    }
}

void opena1(int speed)
{
	TCCR0A |= (1<<WGM00);
	TCCR0A |= (1<<COM0A1);
	OCR0A = speed;
	TCCR0B |= (1<<CS01) | (1<<CS00) | (0<<WGM02);
}