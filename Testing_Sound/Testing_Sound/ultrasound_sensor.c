/*
 * Headerfilemaking.c
 *
 * Created: 23/11/2021 12.54.27
 * Author : danie
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>

void delay_ms (unsigned int);
void delay_us (unsigned int);
void init_getEcho (void);
void reset_prescaler(void);
void disable_interrupt(void);

volatile unsigned long us_counter=0, total_time=0;

unsigned int ultra_sound_sensor(unsigned int sensor){
	unsigned int distance;
	if (sensor == 1)
	{
		DDRD &= ~(1<<DDD2); // Set PINB0 to input -> Echo
		DDRD |= (1<<DDD3); // Set PIND2 to output -> Trigger
		PORTD &= ~(1<<PORTD3);
		delay_us(2);
		PORTD |= (1<<PORTD3);
		delay_us(10);
		PORTD &= ~(1<< PORTD3);
		init_getEcho();
		while (total_time == 0)
		{
				//wait until we not measured the HIGH state
		}
		distance = (total_time * 6) * 0.034 /2;
		total_time = 0;
		delay_ms(60);
	}
	if (sensor == 2)
	{
		DDRD &= ~(1<<DDD4); // Set PINB0 to input -> Echo
		DDRD |= (1<<DDD5); // Set PIND2 to output -> Trigger
		PORTD &= ~(1<<PORTD5);
		delay_us(2);
		PORTD |= (1<<PORTD5);
		delay_us(10);
		PORTD &= ~(1<< PORTD5);
		init_getEcho();
		while (total_time == 0)
		{
			//wait until we not measured the HIGH state
		}
		distance = (total_time * 6) * 0.034 /2;
		total_time = 0;
		delay_ms(60);
	}
	if (sensor == 3)
	{
		DDRD &= ~(1<<DDD6); // Set PINB0 to input -> Echo
		DDRD |= (1<<DDD7); // Set PIND2 to output -> Trigger
		PORTD &= ~(1<<PORTD7);
		delay_us(2);
		PORTD |= (1<<PORTD7);
		delay_us(10);
		PORTD &= ~(1<< PORTD7);
		init_getEcho();
		while (total_time == 0)
		{
			//wait until we not measured the HIGH state
		}
		distance = (total_time * 6) * 0.034 /2;
		total_time = 0;
		delay_ms(60);
	}
	return distance;
	
}
void delay_ms(unsigned int milliseconds){
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	// Set the value that you want to count to
	OCR0A = 0xF9;
	// start the timer
	reset_prescaler();
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
	reset_prescaler();
	TCCR0B |= (1 << CS00);// set pre-scaler to 1 and start the timer
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
	OCR0A = 0x5F;// 0-159 (160 timer ticks)
	//Start the timer
	TIMSK0 |= (1 << OCIE0A); //Set the ISR COMPA vect
	sei(); //enable interrupts;
	reset_prescaler(); //resetting pins
	TCCR0B |= (1 << CS00);// set pre-scaler to 1 and start the timer
}
ISR (TIMER0_COMPA_vect) {
	if (PIND & (1<<PIND2) | PIND & (1<<PIND4) | PIND & (1<<PIND6))
	{
		us_counter++;
	}
	else
	{
		if (us_counter !=0)
		{
			total_time = us_counter;
			us_counter = 0;
			disable_interrupt();
		}
	}
}
void reset_prescaler(void){
	TCCR0B &= ~((1<<CS00) | (1<<CS01));
}
void disable_interrupt(void){
	TCCR0B = 0;
}

