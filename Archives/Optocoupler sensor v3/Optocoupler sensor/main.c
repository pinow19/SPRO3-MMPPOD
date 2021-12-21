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


volatile long  travel_time=0,timer=0,timer_value_holder=0,timer_between_slids=0,t=0;
volatile float avg_rpm=0;
volatile int nr_slids=0,distance=0,flag1=0,flag2=0,nr_slids_before=0;


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
		
		if( (( PIND &= (0<<DDD3) )==0) && (flag1==1))
		{
			nr_slids_before=nr_slids;
			timer_value_holder=travel_time;
			nr_slids++;
			flag1=0;
		}
		
		distance =Wheel_diameter*3.14159*nr_slids/SLID_NR; //this will give us the total distance traveled by the wheeel in cm
		timer=travel_time;
		
	//	_delay_ms(1000);
		printf("the total travel time is %ld ms\n",travel_time);
		printf("actual slids is %d \n",nr_slids);
		//printf("the time between slids in ms is %ld \n",time_holder);
		printf("timer between slids is %ld and t is %ld \n",timer_between_slids,t);
		printf("timer value holder is %ld \n",timer_value_holder);
		printf("distance in cm is %d and the average rpm in rotations/min is %0.2f\n",distance,(float)avg_rpm);
		
    } 
}

void milisec_delay(void)
{
	TCCR0A |= (1<<WGM01);// sets timer to CTC mode
	
	OCR0A = 0xF9;// this value will asure a 1 milisecond delay for a prescalar of 64
	
	TIMSK0|= (1<<OCIE0A);// this line enables the timer0 Compare Match A Interrupt
	
	TCCR0B |= (1<<CS01) | (1<<CS00);// this line sets the value for the prescaler as 64 and it also starts the timer
	
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
		
		milisec_delay();
		flag1=1;
		flag2=1;
	}

    
	
if( ( PIND &= (0<<DDD3) )==0 ) 
   {
	  
	 avg_rpm=(1000*60.0)/t; //average rpm
   
   }
  // printf("WTF \n");
	
}


ISR (TIMER0_COMPA_vect) // timer0 overflow interrupt
{
	sei();
	travel_time++;
	timer_between_slids=travel_time-timer_value_holder;
	if( (( PIND &= (0<<DDD3) )==0) && (flag2==1))
	{
		t=timer_between_slids;
		flag2=0;
	}
}