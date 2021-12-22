#define F_CPU 16000000UL

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include "samd.h"
#include <avr/interrupt.h>

void start_timer(void);
void stop_timer(void);
void setup_motors (void);
void reset_prescaler(void);
uint16_t adc_read(uint8_t adc_channel);

volatile unsigned int ten_us_counter = 0;
volatile unsigned char slid_nr = 0;

unsigned int ultrasound_sensor (unsigned char sensor){
  unsigned char Echo_Pin = sensor;
  unsigned char Trigger_Pin = sensor+2;
  unsigned int distance = 0, duration = 0, timer_LOW = 0, timer_HIGH = 0;
  start_timer();
  PORTB &= ~(1 << Trigger_Pin); // wait 2us to make sure echo is avoided
  delay_us(2);
  PORTB |= (1 << Trigger_Pin); // Activate trigger pin to send out ultrasound noise for 10 us
  delay_us(10);
  PORTB &= ~(1 << Trigger_Pin); //turn off the trigger
  while (!(PINC & (1 << Echo_Pin))){
    timer_LOW = ten_us_counter; } //Wait to get back the signal (reach Rising edge)
  while (PINC & (1 << Echo_Pin)){
    timer_HIGH = ten_us_counter; } //Wait for the signal to finish (reach Falling edge)
  stop_timer();
  duration = timer_HIGH - timer_LOW;
  ten_us_counter = 0;
  distance = duration * 0.34 / 2; // 340m/s -> 0.34 cm/10us because we measure distance every 10 microseconds
  delay_ms(60); //Suggested by datasheet to wait 60ms to ultrasound signal interruption
  return distance;
}

void select_motor_direction (unsigned char direction, unsigned char PWM){
  setup_motors();
  switch (direction)
  {
  case FORWARD:
    PORTD |= (1 << IN1);
    PORTD &= ~(1 << IN2);

    PORTB |= (1 << IN3);
    PORTB &= ~(1 << IN4);

		OCR0A = PWM; //ENB
		OCR0B = PWM; //ENA

    break;

  case BACKWARD:
    PORTD &= ~(1 << IN1);
    PORTD |= (1 << IN2);

    PORTB &= ~(1 << IN3);
    PORTB |= (1 << IN4);

	OCR0A = PWM; //ENB
	OCR0B = PWM; //ENA

    break;
  
  case LEFT:
    //Left goes backward
    PORTD &= ~(1 << IN1);
    PORTD |= (1 << IN2);

    //Right goes forward
    PORTB |= (1 << IN3);
    PORTB &= ~(1 << IN4);

	OCR0A = PWM; //ENB
	OCR0B = PWM; //ENA

    break;
  
  case RIGHT:
    //Left goes forward
    PORTD |= (1 << IN1);
    PORTD &= ~(1 << IN2);

    //Right goes backward
    PORTB &= ~(1 << IN3);
    PORTB |= (1 << IN4);

	OCR0A = PWM; //ENB
	OCR0B = PWM; //ENA

  break;
  
  case STOP:
    PORTD &= ~(1 << IN1);
    PORTD &= ~(1 << IN2);

    PORTB &= ~(1 << IN3);
    PORTB &= ~(1 << IN4);

    OCR0A = 0; //ENB
    OCR0B = 0; //ENA

    break;
  }
}

void setup_motors (void){
  TCCR0A |= (1 << WGM00) | (1 << WGM00); //Enable Fast PWM mode on Timer0
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1); //Enable to put out PWM signal to D5 and D6 pins
  TCCR0B |= (1 << CS00) | (1 << CS01); //Set pre-scaler to 64
}

double analog_sensor (uint8_t adc_channel){
	uint16_t adc_result;
	double result  = 0, subresult[9], finalresult;
	ADMUX = (1<<REFS0); // Select Vref = AVcc
	ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); //set prescaler to 128 and turn on the ADC module
	for (int i = 0; i < 10; i++)
	{
		adc_result = adc_read(adc_channel); //do something with this result…
		subresult[i] = (5000.0/1024)*adc_result;
	}
	for (int i = 0; i < 10; i++)
	{
		result = result + subresult[i];
	}
	finalresult = 2*(result/10000);
	ADCSRA &= ~(1<<ADEN); // Turn back the analog ports to be IO ports
	return finalresult;
}

uint16_t adc_read(uint8_t adc_channel){
	ADMUX &= ~(1 << adc_channel); // clear any previously used channel, but keep internal reference
	ADMUX |= adc_channel; // set the desired channel
	//start a conversion
	ADCSRA |= (1<<ADSC);
	// now wait for the conversion to complete
	while ( (ADCSRA & (1<<ADSC)) );
	// now we have the result, so we return it to the calling function as a 16 bit unsigned int
	return ADC;
}

void optocoupler_slids (unsigned char opto_coupler){
	if (opto_coupler == O_LEFT)
	{
		EICRA |= (1 << ISC01); //Falling edge it activates the interrupt
		EIMSK |= (1 << INT0); // enable INT0 -> PD2
		sei(); 		
	} 
	else
	{
		EICRA |= (1 << ISC11); //Falling edge it activates the interrupt
		EIMSK |= (1 << INT1); //enable  INT1 -> PD3
		sei();
	}
}


ISR (INT0_vect) // external interrupt 0{
	slid_nr ++;	
}

ISR (INT1_vect)
{
	slid_nr ++;
}

void reset_slid_nr_and_stop_interrupt(void){
	slid_nr = 0;
	EIMSK &= ~(1 << INT0); //Disable interrupt0
	EIMSK &= ~(1 << INT1); //Disable interrupt1
}

void delay_ms (unsigned int ms){
  TCCR2A |= (1 << WGM21); // Set the Timer2 Mode to CTC
  OCR2A = 0xF9; // count up to 256 ticks (0-255) to reach 1ms
  reset_prescaler();
  TCCR2B |= (1 << CS22); // set pre-scaler to 64 and start the timer
  for (int i=0; i < ms; i++){
    while (!(TIFR2 & (1 << OCF2A))); // wait for the overflow event
    TIFR2 |= (1 << OCF2A);
  }
}

void delay_us (unsigned int us){
  TCCR2A |= (1 << WGM21); // Set the Timer2 Mode to CTC
  OCR2A = 0x0F; // count up to 16 ticks (0-15) to reach 1us
  reset_prescaler();
  TCCR2B |= (1 << CS20); // set pre-scaler to 1 and start the timer
  for (int i=0; i < us; i++){
    while (!(TIFR2 & (1 << OCF2A))); // wait for the overflow event
    TIFR2 |= (1 << OCF2A);
  }
}

void start_timer (void){
	TCCR1A = 0;
	TCCR1B |= (1 << WGM12); // Set the Timer1 Mode to CTC
	OCR1A = 0x9F; //160 tick (0-159) to reach 10us
	TIMSK1 |= (1 << OCIE1A); //Set the ISR COMPA vect
	sei();
	TCCR1B |= (1 << CS10); // set pre-scaler to 1 to reach us
}

ISR (TIMER1_COMPA_vect){
  ten_us_counter++;
  if(ten_us_counter>65000) ten_us_counter = 0; //to avoid overflow of int
}

void stop_timer (void){
  TCCR1B = 0; //stop interrupt
}

void reset_prescaler(void){
  TCCR2B &= ~((1 << CS20) | (1 << CS22));
  }

void set_pins (void){
  //Setting Echo pins
  DDRC &= ~(1 << U_FRONT); //A0 set as input
  DDRC &= ~(1 << U_LEFT); //A1 set as input
  DDRC &= ~(1 << U_RIGHT); // A2 set as input
  
  //Setting Voltage and Temperature analog sensors
  DDRC &= ~(1 << TEMP_SENSOR);
  DDRC &= ~(1 << VOLTAGE_SENSOR);

  //Setting Trigger pins
  DDRB |= (1 << (U_FRONT+2)); //D10 set as output
  DDRB |= (1 << (U_LEFT+2)); //D11 set as output
  DDRB |= (1 << (U_RIGHT+2)); //D12 set as output

  //Setting Optocoupler PINS
  DDRD &= ~(1 << O_LEFT); //D2 set as input
  DDRD &= ~(1 << O_RIGHT); //D3 set as input

  //Setting motor pins
  DDRD |= (1 << IN1); //D4 set as output
  DDRD |= (1 << IN2); //D7 set as output
  DDRB |= (1 << IN3); //D8 set as output
  DDRB |= (1 << IN4); //D9 set as output
  DDRD |= (1 << ENA); //D5 set as output
  DDRD |= (1 << ENB); //D6 set as output
}
//info: https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/