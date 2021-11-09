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
#include <avr/interrupt.h>

void delay_us(unsigned char);
void init_interrupt();
volatile unsigned int us_counter;
char sendsignal=1;
volatile float distance=0;

int main(void)
{
	uart_init();
	io_redirect();
	init_interrupt();
	
	DDRB &= ~(1 << DDB0);
	DDRD |= (1 << DDD2);
	
    /* Replace with your application code */
    while (1) 
    {
		if(sendsignal==1){
		PORTD &= (0<<PORTD2);
		delay_us(2);
		PORTD |= (1<<PORTD2);
		delay_us(10);
		PORTD &= (0<<PORTD2);
		sendsignal=0;
		}
		printf("The result: %f \n", distance);
	}
}

void delay_us(unsigned char us)
{
	for(int i =0; i<us; i++){
		// configure the timer to CTC mode
		TCCR0A |= (1<<WGM01);
		// set desired number of states we want
		// we want 16 states
		OCR0A = 0x0F;
		// set prescaler and start timer
		TCCR0B =(1<<CS00); //CS00 = 1 --> prescaler of 1
		// wait for match between current state and desired state
		while ((TIFR0 & (1 << OCF0A))==0) //means flag is low
		{
			//do nothing
		}
		// I know that at this point in code, 1 us has passed.
		// reset match flag by adding 1 into the flag
		TIFR0 = (1 << OCF0A);
	}
}

void init_interrupt(void){
	//set the timer mode to CTC <- count to desired value and then restart and count again
	TCCR0A |= (1 << WGM01);
	//set the value that you want to count to <- 250 timer ticks (0-249)
	OCR0A = 0x0F;
	
	//enable the interrupt for on compare a for timer 0
	TIMSK0 |= (1 << OCIE0A); //can be interrupted by Compare A matrch
	//enable all interrupts
	sei();
	
	//start the timer
	TCCR0B |= (1 << CS00); //set prescaler to 64 and start
}

ISR (TIMER0_COMPA_vect) {
	if (PINB & (1<<PINB0)) us_counter++;
	if (~(PINB & (1<<PINB0)))
		distance = us_counter * 0.034 / 2;
		sendsignal=1;
		us_counter++;
	}