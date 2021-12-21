/*
 * Optocoupler sensor.c
 *
 * Created: 09/11/2021 13.30.40
 * Author : Mihau Gruia
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include "usart.h"
#include <util/delay.h>
#include <avr/interrupt.h>

#define SLID_NR 6
#define Wheel_diameter 6
void milisec_delay(void);
void interrupt_enabler(void);
//void passing_time(void)

volatile long  travel_time=0,slid_time=0,time_holder=0,timer=0;
float avg_rpm=0;
volatile int nr_slids=0,actual_slids=0,distance=0,flag=0;


int main(void)
{
	uart_init();
	io_redirect();
	
	DDRD &= ~(1<<DDD3);// PORTD3 becomes an input pin
    PORTD &=~(1<<DDD3);  
	interrupt_enabler();
    while (1) 
    {
		milisec_delay();
		//actual_slids=nr_slids/2;
		if( (( PIND &= (0<<DDD3) )==0) && (flag==1))
		{
			actual_slids++;
			flag=0;
		}
		slid_time++;
		distance =Wheel_diameter*3.14159*actual_slids/SLID_NR; //this will give us the total distance traveled by the wheeel in cm
		timer=travel_time;
		avg_rpm=(actual_slids*1000*60.0)/timer; //average rpm
	//	_delay_ms(1000);
		printf("the total travel time is %ld ms\n",travel_time);
		printf("actual slids is %d \n",actual_slids);
		//printf("the time between slids in ms is %ld \n",time_holder);
		printf("distance in cm is %d and the average rpm in rotations/min is %0.2f\n",distance,(float)avg_rpm);
		
    } 
}

void milisec_delay(void)
{
	TCCR0A |= (1<<WGM01);// sets timer to CTC mode
	
	OCR0A = 0xF9;// this value will asure a 1 milisecond delay for a prescalar of 64
	
	TIMSK0|= (1<<OCIE0A);// this line enables the timer0 Compare Match A Interrupt
	
	TCCR0B |= (1<<CS01) | (1<<CS00);// this line sets the value for the prescaler as 64 and it also starts the timer
	
	

	/*for(int i=0;i<time;i++) 
	{
		while ( (TIFR0 & (1 << OCF0A) ) == 0) // wait for the overflow event
		{
			
		}
		TIFR0 = (1 << OCF0A);// resets overflow flag
		
	}*/
	
}

void interrupt_enabler(void)
{
	EICRA |= (1<<ISC11);// only the falling edge will generate an interrupt request on the interrupt
	
	EICRA &= ~(1<<ISC10);
	
	EIMSK |= (1<<INT1);// enables external interrupt number 1 (PD3)
	
	EIMSK &= ~(1<<INT0);
	
	sei();
}

ISR (INT1_vect)// external interrupt 1
{
	while( ( PIND &= (1<<DDD3) ) == 0 )
	{
		sei();
		//travel_time++;
		//cli();
		milisec_delay();
	}
	//sei();
//	timer++;
//	milisec_delay(1);
    time_holder=slid_time;
    slid_time=0;
  //  nr_slids++;
	flag=1;
}


ISR (TIMER0_COMPA_vect) // timer0 overflow interrupt
{
	travel_time++;
}