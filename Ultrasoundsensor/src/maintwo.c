/*
 * Ultrasonic_Sensor.c
 *
 * Created: 29/10/2021 14.57.40
 * Author : Dani Sunca
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "usart.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void delay_us(unsigned int microseconds);

int main(void)
{
	uart_init();
	io_redirect();
	
	DDRD &= ~(1<<DDD3); //Echo Pin set as input (PD3 = PCINT19)
	DDRD |= (1<<DDD2); //Trigger Pin set as output (PD2 = PCINT18)
	unsigned int distance, duration = 0;
	float total_time;
	/* Replace with your application code */
    while (1) 
    {
		PORTD &= ~(1<<PORTD2); //Trigger pin is low
		delay_us(2);
		PORTD |= (1<<PORTD2); //Trigger pin is high
		delay_us(10);
		PORTD &= ~(1<<PORTD2); //Trigger pin is low
		while (PIND & (1 << PIND3) == 1)
		{
			duration++; // adding operation takes 1 cycle of 16Mhz CPU, so we just need to multiply by 1/16Mhz*10E6 to get the duration in microseconds
		}
		
		printf("duration: %u", duration);
		total_time = duration * 0.0625;
		printf("Total time: %f", total_time);
		duration = 0;
		distance = total_time * 0.034 / 2;
		printf("Distance : %d cm\n", distance);
		_delay_ms(60);
    }
}
void delay_us(unsigned int microseconds){
    TCCR0A |= (1 << WGM01); //Set the Timer mode to CTC (=Clear on Compare)
    OCR0A = 0x0F; //setting timer from 0 to 15 (16)
    TCCR0B |= (1 << CS00);// set no pre-scaler start the timer
    /*
    Why we got no pre-scaler because: OCRn=[(clock_speed/prescaler)*time_in_seconds]-1
    to get 1microsecond OCRn=[(1.6*10E7/1)*10E-6]-1 ->15  -> 16MHz = 1.6*10E7 Hz
    */
   for (int i=0; i<microseconds; i++)
	{
		while ( (TIFR0 & (1 << OCF0A) ) == 0) // wait for the overflow event
		{
		}
		TIFR0 = (1 << OCF0A); // reset the overflow flag
	}
}
