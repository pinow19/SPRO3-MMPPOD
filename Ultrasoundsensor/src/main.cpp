#include <Arduino.h>
#include "usart.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define Trig_Pin PCINT21
#define Echo_Pin PCINT22
#define HIGH 1
#define LOW 0

void delay_us(unsigned int microseconds);
void getEcho (void);
volatile unsigned int duration;

int main(void){
    uart_init();
    io_redirect();
    DDRD &= (LOW<<Echo_Pin); //setting pin to zero -> input
    DDRD |= (HIGH<<Trig_Pin); //setting pin to one -> output
    unsigned int distance;
    

    while (1){
        PORTD &= (LOW<<Trig_Pin);
        delay_us(2);
        PORTD |= (HIGH<<Trig_Pin);
        delay_us(10);
        PORTD |= (LOW<<Trig_Pin);
        PORTD &= (LOW<<Echo_Pin);
        getEcho();
        distance = duration * 0.034 / 2;
        printf("Distance: %d\n", distance);
    }
}

void delay_us(unsigned int microseconds){
    TCCR0A |= (HIGH << WGM01); //Set the Timer mode to CTC (=Clear on Compare)
    OCR0A = 0xF; //setting timer from 0 to 15 (16)
    TCCR0B |= (HIGH << CS00);// set no prescaler start the timer
    /*
    Why we got no pre-scaler because: OCRn=[(clock_speed/prescaler)*time_in_seconds]-1
    to get 1microsecond OCRn=[(1.6*10E7/1)*10E-6]-1 ->15  -> 16MHz = 1.6*10E7 Hz
    */
   	for (int i=0; i<microseconds; i++)
	{
		while ( (TIFR0 & (HIGH << OCF0A) ) == LOW) // wait for the overflow event
		{
		}
		TIFR0 = (HIGH << OCF0A); // reset the overflow flag
	}
}

void getEcho(void){
    TCCR2A=(1<<WGM01);    //Set the CTC mode   
    OCR2A=0xF; //Value for ORC0A for 1microsecond 
  
    TIMSK2|=(1<<OCIE2A);   //Set the interrupt request
    TCCR2B|=(1<<CS20);    //Set to no prescale   
    duration = 0;
    sei(); //Enable interrupt
    while (PIND != (HIGH<<Echo_Pin)){} //wait until the Echo Pin is not high
    cli(); //Disable interrupt
}
ISR (TIMER2_COMPA_vect){ // interrupt will fire every 1 microseconds
    duration++;
}
 