#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "usart.h"
#include <avr/interrupt.h>

#define PoM  6 //Points of Measurement - how many holes the optowheel has

volatile int count = 0, count_off = 0;
double rpm = 0;
int slids = 0;


void init_interrupt(void);

int main(void)
{
	uart_init();
	io_redirect();
	
	DDRD &= ~(1<<DDD3); //clear the pin PD3
	DDRD |= 1<<DDD3; //set this pin as input
	
    while (1)
    {
		if (PIND & 1<<PIND3) //initial contact, but has to wait for the ripple effect
		{
			if(PIND & 1<<PIND3) //check again
			{
				while(PIND & 1<<PIND3) //start counting the time
				{
					_delay_ms(1); //so that everything is a bit slowed down, otherwise it overflows
					count++;
				}		
				rpm = 600000/(((double)count+(double)count_off)*PoM); //calculate the rpm - needs adjustment as the time does not acccount for the code load
	
				slids++;
				count_off=0;
					
				printf("%f %d\n", rpm, slids);
			}
		}
		_delay_ms(1);
		count_off++;
		count = 0;

	}

}
