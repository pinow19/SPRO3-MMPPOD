/*
 * Opto_coupler.c
 *
 * Created: 20/11/2021 15.28.22
 * Author : danie
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>
#include "usart.h"

#define SLID_NR 6

void setupPINs(void);
int getRPM(void);
void startTimer(void);
void stopTimer(void);
void start_count_slids (void);
void reset_counted_slids(void);

volatile unsigned long getTime = 0, pin_position, counted_slids=0;


int main(void)
{
	uart_init();
	io_redirect();
	
	setupPINs();
	int RPM;
    while (1) 
    {
		RPM = getRPM();
		start_count_slids();
		printf("RPM: %d, Slids: %ld\n", RPM, counted_slids);
    }
}

void setupPINs(void){
	DDRD &= ~(1<DDD3); // Setup PD3 to input
	PORTD |= (1<<PORTD3); //Enable pull-up for PD3
}

int getRPM(void){
	volatile unsigned long TimeHIGH, TimeLOW;
	for (int i=0; i<2; i++)
	{
		pin_position=0;
		
		if (PIND & (1<<DDD3)) //if the PIN is high
		{
			pin_position=1;
		}
		else
		{
			pin_position=2;
		}
		if (pin_position==1)
		{
			while(PIND & (1<<DDD3)){} // wait to get LOW
			startTimer(); // count the low time
			while(~(PIND & (1<<DDD3))){} // wait to get HIGH
			stopTimer();
			TimeLOW = getTime*6;
			getTime = 0;
			
			
		}
		if (pin_position==2)
		{
			while(~(PIND & (1<<DDD3))){} // wait to get HIGH
			startTimer(); // count the low time
			while(PIND & (1<<DDD3)){} // wait to get LOW;
			stopTimer();
			TimeHIGH = getTime*6;
			getTime = 0;
		}
	}
	return (6E7/((TimeHIGH+TimeLOW)*SLID_NR)); // returns RPM
	

}

void startTimer (void){
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	// Set the value that you want to count to
	OCR0A = 0x5F;// 0-95 (96 timer ticks) -> 6 microseconds
	//Start the timer
	TIMSK0 |= (1 << OCIE0A); //Set the ISR COMPA vect
	sei(); //enable interrupts;
	TCCR0B |= (1 << CS00);// set pre-scaler to 1 and start the timer
}
ISR (TIMER0_COMPA_vect) {
		getTime++;
}
	

void stopTimer (void){
	TCCR0B = 0;
}

void start_count_slids (void){
	PCICR |= (1 << PCIE2); // set PCIE2 to enable the group for PCINT23..PCINT16 The PD3 is PCINT[19]
	PCMSK2 |= (1 << PCINT19); // Enable only PCINT19 [PD3] interrupt from the group
	sei(); // turn on interrupts
}

ISR (PCINT2_vect){
	if (PIND & (1<<PIND3))
	{
		counted_slids++;
	}
}

void reset_counted_slids(void){
	counted_slids = 0;
}
