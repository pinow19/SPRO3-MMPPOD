#define F_CPU 16E6
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"

#define CW 1
#define CCW 2
#define BRAKE 3
#define number_of_motors 2

struct Motors_t{
	unsigned int pwm_value;
	int direction;
	int state[2];
};

void setup(void);

struct Motors_t M[number_of_motors];

int main()
{
	uart_init();
	io_redirect();
	setup();
	
	while (1)
	{
		setup();
		
		
		for(int i=0;i<number_of_motors;i++)
		{
			printf("\nPlease give Motor #%d PWM cycle:", i+1);
			scanf("%d", &M[i].pwm_value);
			printf("\nPlease give Motor #%d direction:", i+1);
			scanf("%d", &M[i].direction);
			switch (M[i].direction)
			{
				case CW: M[i].state[0] = 1; M[i].state[1] = 0; break;
				case CCW: M[i].state[0] = 0; M[i].state[1]= 1; break;
				case BRAKE: M[i].state[0] = 0; M[i].state[1]= 0; break;
			}
		}
		PORTD &= ~((1<<PORTD2) | (1<<PORTD4)); //THIS IS BAD
		PORTB &= ~((1<<PORTB0) | (1<<PORTB4)); //FIX THIS
		OCR0A = M[0].pwm_value;
		OCR0B = M[1].pwm_value;
		PORTD |= (((M[0].state[0]<<PORTD2) | (M[0].state[1]<<PORTD4)));
		PORTB |= (((M[1].state[0]<<PORTB0) | (M[1].state[1]<<PORTB4)));
			
		printf("%d %d %d %d %d %d", M[0].pwm_value, M[1].pwm_value, M[0].state[0], M[0].state[1], M[1].state[0], M[1].state[1]);
	}
}

void setup() {
	DDRD |= (1<<DDD5)|(1<<DDD6); //PWM signals
	DDRD |= (1<<DDD2)|(1<<DDD4); //Direction for motor 1
	DDRB |= (1<<DDB0)|(1<<DDB4); //Direction for motor 2
	
	TCCR0A = (1<<WGM01)|(1<<WGM00); //fast pwm mode
	TCCR0A|= ((1<<COM0A1) | (1<<COM0B1)); //non-inverting something
	
	TCCR0B = (1<<CS22); //prescaler = 64
}

