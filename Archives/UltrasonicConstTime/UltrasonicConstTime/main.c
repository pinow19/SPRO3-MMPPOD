/*
 * UltrasonicConstTime.c
 *
 * Created: 03-12-2021 14:57:14
 * Author : Marcel
 */ 

#define F_CPU 16E6
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include <avr/interrupt.h>

volatile unsigned int count1;
volatile char started1 = 0;

void init_interrupt(void);

int main(void)
{
	uart_init();
	io_redirect();
		
	DDRD &= ~(1<<DDD2); // Set PINB0 to input -> Echo
	DDRD |= (1<<DDD3); // Set PIND2 to output -> Trigger
	
    init_interrupt();

	unsigned int distance;
	
	PORTD &= ~(1<<PORTD3);
	
    while (1) 
    {
		volatile int pind2=(PIND & (1<<PIND2));
		//if (count1>50)
		//{
		//	printf("%u %d %d\n", count1, pind2, started1);
		//}
		//printf("%u %d %d\n", count1, pind2, started1);
		//if(started1==1)
		if(count1>10)printf("%d\n", count1);
		//if(started1==0) PORTD |= (1<<PORTD3);
		//else PORTD &= ~(1<< PORTD3);
		
    }
}

void init_interrupt(void)
{
	TCCR0A |= (1<<WGM01); //CTC Mode
	OCR0A = 19; //10us with prescaler of 8
	//so far only setting up the timer
	TIMSK0 |= (1<<OCIE0A); //This tells the interrupt that the flag of the timer has been raised (a doorbell)
	sei(); //enable all interrupts
	TCCR0B |= (1<<CS01);// | (1<<CS00); //start the timer with prescaler of 8
}


ISR (TIMER0_COMPA_vect)
{
	/*if(!(PIND & (1<<PIND2)) && started1==0)
	{
		PORTD |= (1<<PORTD3);
		//printf("p1\n");
	}
	else if((PIND & (1<<PIND2)) && started1==0)
	{
		started1=1;
		count1=0;
		PORTD &= ~(1<< PORTD3);
		//printf("p2\n");
	}
	else if((PIND & (1<<PIND2)) && started1==1)count1++;
	else if(!(PIND & (1<<PIND2)) && started1==1)
	{
		PORTD &= ~(1<<PORTD3);
		started1=0;
		printf("p3\n");
	}*/
	if((PIND & (1<<PIND2))!=4)
	{
		started1=0;
		PORTD |= (1<<PORTD3);
	}
	else if(((PIND & (1<<PIND2))==4) && started1==0)
	{
		started1=1;
		count1=0;
		PORTD &= ~(1<< PORTD3);
	}
	else if(((PIND & (1<<PIND2))==4) && started1==1)count1++;
	else if(((PIND & (1<<PIND2))!=4) && started1==1)
	{
		started1=0;
		PORTD &= ~(1<< PORTD3);
	}
}

