#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "usart.h"
#include <avr/interrupt.h>

#define PoM  6

volatile int count = 0, count_off = 0;
double rpm = 0;
int slids = 0;


void init_interrupt(void);

int main(void)
{
	uart_init();
	io_redirect();
	
	DDRD &= ~(1<<DDD3); //clear the pin PD2
	DDRD |= 1<<DDD3;
	
    while (1)
    {
		if (PIND & 1<<PIND3)
		{
			if(PIND & 1<<PIND3)
			{
				while(PIND & 1<<PIND3)
				{
					_delay_ms(1);
					count++;
				}		
				rpm = 600000/(((double)count+(double)count_off)*PoM);
	
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