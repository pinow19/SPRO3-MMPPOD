#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "usart.h"
#include <avr/interrupt.h>
#include "opto.h"

#define PoM  6

volatile int count_opto = 0, count_off = 0;
double rpm = 0;
int slids = 0;
int flag_opto = 0;


int opto_turn(int turns, int side)
{
	
	slids = 0;	
	//DDRD &= ~(1<<DDD3); //clear the pin PD2
	//DDRD |= 1<<DDD3;
	
	 
	
    while (slids<turns)
    {
		
		if (PIND & 1<<side)
		{
			
			if(PIND & 1<<side)
			{
				while(PIND & 1<<side)
				{
					_delay_ms(1);
					count_opto++;
				}		
				rpm = 600000/(((double)count_opto+(double)count_off)*PoM);
	
				slids++;
				count_off=0;
					
				
			}
		}
		_delay_ms(1);
		count_off++;
		count_opto = 0;

	}

}