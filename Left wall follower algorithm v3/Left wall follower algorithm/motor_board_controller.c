#include <avr/io.h>
#include <stdio.h>
#include "motor_board_controller.h"



void setup() {
	DDRB |= (1<<DDB1)|(1<<DDB2); //PWM signals
	DDRB |= (1<<DDB3)|(1<<DDB5); //Direction for motor 1
	DDRB |= (1<<DDB0)|(1<<DDB4); //Direction for motor 2
	
	TCCR1A = (1<<WGM11)|(1<<WGM10); //fast pwm mode
	TCCR1A|= ((1<<COM1A1) | (1<<COM1B1)); //non-inverting something
	
	TCCR1B = (1<<CS12); //prescaler = 64
}
void motor_setup()
{
	for(int i=0;i<number_of_motors;i++)
	{
		printf("\nPlease give Motor #%d PWM cycle:", i+1);
		scanf("%d", &M[i].pwm_value);
		/*printf("\nPlease give Motor #%d direction:", i+1);
		scanf("%d", &M[i].direction);
		switch (M[i].direction)
		{
			case CW: M[i].state[0] = 1; M[i].state[1] = 0; break;
			case CCW: M[i].state[0] = 0; M[i].state[1]= 1; break;
			case BRAKE: M[i].state[0] = 0; M[i].state[1]= 0; break;
		}*/
	}
	OCR1A = M[0].pwm_value;
	OCR1B = M[1].pwm_value;
	//PORTD = 0; //THIS IS BAD
	//PORTB = 0; //FIX THIS
	PORTB |= (((M[0].state[0]<<PORTB3) | (M[0].state[1]<<PORTB5)));
	PORTB |= (((M[1].state[0]<<PORTB0) | (M[1].state[1]<<PORTB4)));
	
	printf("%d %d %d %d %d %d", M[0].pwm_value, M[1].pwm_value, M[0].state[0], M[0].state[1], M[1].state[0], M[1].state[1]);
}