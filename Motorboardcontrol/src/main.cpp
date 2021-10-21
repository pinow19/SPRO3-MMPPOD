#include <Arduino.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#define CW 1
#define CCW 2
#define BRAKE 3
#define number_of_motors 6

void pwm_init (void);
void motor_state (void);
void set_pwm_signal (void);

void send_data_to_chip (void);

struct Motors_t{
	unsigned int pwm_value;
	unsigned int IN[2];
	unsigned int motorstate;
	};

struct Motors_t M[number_of_motors];

unsigned int motorstate_array[12];

int main(void){
//PWM pins set as outputs
DDRB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB3);
DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);

// Latch, Clock and Data pins for 74HC595 set as outputs
DDRD |= (1 << DDD2) | (1 << DDD4) | (1 << DDD7);



    while (1) 
    {
		set_pwm_signal();
		motor_state();
		pwm_init();
		send_data_to_chip();
		return 0;
    }
}
void pwm_init(void) {
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // enable fast PWM mode and PD3
	TCCR0B |= (1 << CS00) | (1 << CS01); // pre-scaler 64	
	
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10); // enable fast PWM mode and PD3
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS11); // pre-scaler 64
	
	TCCR2A |= (1<< COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20); // enable fast PWM mode and PD3
	TCCR2B |= (1 << CS22); // pre-scaler 64
	
	OCR2B = M[0].pwm_value; //D3 pin -> MotorBoard-1 enA
	OCR0B = M[1].pwm_value; //D5 pin -> MotorBoard-1 enB
	OCR0A = M[2].pwm_value; //D6 pin -> MotorBoard-2 enA
	OCR1A = M[3].pwm_value; //D9 pin -> MotorBoard-2 enB
	OCR1B = M[4].pwm_value; //D10 pin -> MotorBoard-3 enA
	OCR2A = M[5].pwm_value; //D11 pin -> MotorBoard-3 enB
	}
	
void set_pwm_signal(){
	for (int i=0; i<6; i++)
	{
		printf("\nPlease give Motor #%d PWM cycle:", i+1);
		scanf("%d", &M[i].pwm_value);
	}
	}

void motor_state (void){
	for (int i=0; i<6; i++)
		{
			printf("\nPlease give Motor #%d motorstate CW/CCW/BRAKE:", i+1);
			scanf("%d", &M[i].motorstate);
		switch (M[i].motorstate){
			case CW: M[i].IN[0] = 1; M[i].IN[1] = 0; break;
			case CCW: M[i].IN[0] = 0; M[i].IN[1] = 1; break;
			case BRAKE: M[i].IN[0] = 0; M[i].IN[1] = 0; break;
			}
		}
	motorstate_array[0] = M[5].IN[1];
	motorstate_array[1] = M[5].IN[0];
	motorstate_array[2] = M[4].IN[1];
	motorstate_array[3] = M[4].IN[0];
	motorstate_array[4] = M[3].IN[1];
	motorstate_array[5] = M[3].IN[0];
	motorstate_array[6] = M[2].IN[1];
	motorstate_array[7] = M[2].IN[0];
	motorstate_array[8] = M[1].IN[1];
	motorstate_array[9] = M[1].IN[0];
	motorstate_array[10] = M[0].IN[1];
	motorstate_array[11] = M[0].IN[0];
}

void send_data_to_chip (void){
	PORTD &= (0 << DDD7); // Latch is on low
	
	for (int i=11; i<=0; i--)
	{
		PORTD &= (0 << DDD4); //Clock is on low
		if (motorstate_array[i] == 0) PORTD &= (0 << DDD2); //Send 0 to data pin
		if (motorstate_array[i] == 1) PORTD |= (1 << DDD2); //Send 1 to data pin
		PORTD |= (1 << DDD4); //Clock is on high
	}
	PORTD |= (1 << DDD7); // Latch is on high
}