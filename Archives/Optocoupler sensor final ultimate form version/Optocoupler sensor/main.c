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

#define SLID_NR 6   // the number of slids present on the optocoupler wheel
#define Wheel_diameter 6 // the diameter in cm
void milisec_delay(void);
void interrupt_enabler(void);
void Pinsetup(void);
void slid_counter(void);
void slids_per_min(void);
void show_values(void);
char x = 0; //to decrease printf delay

volatile long  travel_time=0,timer=0,timer_value_holder=0,timer_between_two_slids=0,t=0;
volatile float avg_slids_per_min=0,avg_rpm=0;
volatile int nr_slids=0,distance=0,flag1=0,flag2=0,nr_slids_before=0;
volatile uint8_t pin_holder;

int main(void)
{
	
	uart_init();
	io_redirect();
	
	Pinsetup(); 
	interrupt_enabler();
	milisec_delay();
	
    while (1) 
    {
		
		
		
		slid_counter();
		
		distance =Wheel_diameter*3.14159*nr_slids/SLID_NR; //this will give us the total distance traveled by the wheeel in cm
		
		slids_per_min();
		
		avg_rpm=avg_slids_per_min/SLID_NR;
			
		show_values();
		
    } 
}
void Pinsetup(void)
{
	DDRD &= ~(1<<DDD3);// PORTD3 becomes an input pin
	PORTD |=(1<<DDD3);// pull up resistor
}

void milisec_delay(void)
{
	TCCR0A |= (1<<WGM01);// sets timer to CTC mode
	
	OCR0A = 0xF9;// this value will assure a 1 millisecond delay for a prescaler of 64
	
	TIMSK0|= (1<<OCIE0A);// this line enables the timer0 Compare Match A Interrupt
	
	TCCR0B |= (1<<CS01) | (1<<CS00);// this line sets the value for the prescaler as 64 and it also starts the timer
	
}

void interrupt_enabler(void)
{
	EICRA |= (1<<ISC11);// only the falling edge will generate an interrupt request on the interrupt
	
	EICRA &= ~(1<<ISC10);//just making sure that i have a falling edge
	
	EIMSK |= (1<<INT1);// enables external interrupt number 1 (PD3)
	
	EIMSK &= ~(1<<INT0);// deactivates external interrupt number 0 (PD2)
	
	sei();
}

void slid_counter(void)
{
	
	
	pin_holder=PIND;// holds the value of the PIND
	
	if( (( PIND &= ~(1<<DDD3) )!=pin_holder) && (flag1==1))// i am checking if the optocopler sends a HIGH signal and if the flag1 is 1 
	{
		nr_slids_before=nr_slids;
		timer_value_holder=travel_time;// variable that stores the time
		nr_slids++;// total number of slids detected
		flag1=0;
	}
}

void slids_per_min(void)
{
	timer=travel_time;
	pin_holder=PIND;
	if( (( PIND &= ~(1<<DDD3) )!=pin_holder) && (flag2==1) )// checks if the optocopler sends a HIGH signal and if the flag2 is 2
	{
		t=timer_between_two_slids;// the time between 2 slids
		flag2=0;
		
		avg_slids_per_min=(1000*60.0)/timer_between_two_slids; //average slids/min
		
	}
}


void show_values()
{
	if(x==1) x=2;
	if(x==0){
	printf("The total travel time is %ld ms\n",travel_time);
	printf("The number of slids detected is %d \n",nr_slids);
	//printf("timer between slids is %ld and t is %ld \n",timer_between_two_slids,t);
//	printf("The value that is stored in timer value holder is %ld \n",timer_value_holder);
	 printf("Distance traveled in cm is %d and the average nr of slids/min is %0.2f\n",distance,(float)avg_slids_per_min);
	//printf("The average rpm is %0.2f rotations/min \n",avg_rpm);
	x=1;
	}
	if(x==2) x=0;
}

ISR (INT1_vect)// external interrupt 1
{
	pin_holder=PIND;
	while( ( PIND &= ~(1<<DDD3) ) == pin_holder )// the optocopler detects the slid and sends a LOW signal
	{
	  if(PIND!=pin_holder)// the optocupler detects the wheel
	  {
	     pin_holder=PIND;//
	  }
		flag1=1;
		flag2=1;
	}
	
}


ISR (TIMER0_COMPA_vect) // timer0 overflow interrupt
{
	
	travel_time++;
	timer_between_two_slids=travel_time-timer_value_holder;
}