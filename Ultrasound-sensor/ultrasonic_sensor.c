//!!Important in Atmel/Microchip Studio set printf_flt and avrchain to show floating numbers!!!

/*
 * FinishingSensorUp.c
 *
 * Created: 12/11/2021 09.47.43
 * Author : danie
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "usart.h"
#include <math.h>

void delay_ms (unsigned int);
void delay_us (unsigned int);
void init_getEcho (void);

volatile unsigned int us_counter=0, total_time=0;

int main(void)
{
	uart_init();
	io_redirect();
	DDRB &= ~(1<<DDB0); // Set PINB0 to input -> Echo
	DDRD |= (1<<DDD2); // Set PIND2 to output -> Trigger
	float distance;
	/* Replace with your application code */
	while (1)
	{
		init_getEcho();
		PORTD &= (0<<PORTD2);
		delay_us(2);
		PORTD |= (1<<PORTD2);
		delay_us(10);
		PORTB &= (0<< PORTD2);
		while (total_time == 0)
		{
			//wait until we not measured the HIGH state
		}
		cli();
		distance = (total_time*0.034)/2;
		total_time = 0;
		printf("Distance: %f\n", distance);
		delay_ms(60);
	}
}
void delay_ms(unsigned int milliseconds){
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	// Set the value that you want to count to
	OCR0A = 0xF9;
	// start the timer
	TCCR0B &= ~(1 << CS01);
	TCCR0B &= ~(1 << CS00);
	//resetting pins
	TCCR0B |= (1 << CS01) | (1 << CS00);// set pre-scaler to 64 and start the timer
	for (int i=0; i<milliseconds; i++)
	{
		while ( (TIFR0 & (1 << OCF0A) ) == 0) // wait for the overflow event
		{
		}
		// reset the overflow flag
		TIFR0 = (1 << OCF0A);
	}
}
void delay_us(unsigned int microseconds){
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	// Set the value that you want to count to
	OCR0A = 0x0F;
	// start the timer
	TCCR0B &= ~(1 << CS01);
	TCCR0B &= ~(1 << CS00);
	//resetting pins
	TCCR0B |= (1 << CS00);// set prescaler to 1 and start the timer
	for (int i=0; i<microseconds; i++)
	{
		while ( (TIFR0 & (1 << OCF0A) ) == 0) // wait for the overflow event
		{
		}
		// reset the overflow flag
		TIFR0 = (1 << OCF0A);
	}
}
void init_getEcho (void){
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	// Set the value that you want to count to
	OCR0A = 0x0F;// 0-15 (16 timer ticks)
	//Start the timer
	TIMSK0 |= (1 << OCIE0A); //Set the ISR COMPA vect
	sei(); //enable interrupts
	TCCR0B &= ~(1 << CS01);
	TCCR0B &= ~(1 << CS00);
	//resetting pins
	TCCR0B |= (1 << CS00);// set pre-scaler to 1 and start the timer
}
ISR (TIMER0_COMPA_vect) {
	if (PINB & (1<<PINB0))
	{
		us_counter++;
	}
	else
	{
		if (us_counter !=0)
		{
			total_time = us_counter;
			us_counter = 0;
			cli();
		}
	}
}

