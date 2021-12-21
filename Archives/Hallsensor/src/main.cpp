#include <Arduino.h>
#include "usart.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

uint16_t adc_read(uint8_t adc_channel);
uint16_t adc_result;

int main(void){
  DDRC = 0xF0; //A0 to A3 set to input for ADC
  PORTC = 0x00; //disable pull-ups

  ADMUX = (1<<REFS0); //Select Vref to AVcc
  ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); //set pre-scaler to 128 and turn on the ADC module
  int COS_N = 0, COS_P = 0, SIN_N = 0, SIN_P = 0;

  while(1){
    for (uint8_t channel = 0; channel < 4; channel++){
      adc_result = adc_read(channel);
		  int voltage = (5000.0/1024)*adc_result;
      if (channel == 0) COS_N = voltage;
      if (channel == 1) COS_P = voltage;
      if (channel == 2) SIN_N = voltage;
      if (channel == 3) SIN_P = voltage;
      printf("Channel: COS_N = %d\n", COS_N);
      printf("Channel: COS_N = %d\n", COS_P);
      printf("Channel: COS_N = %d\n", SIN_N);
      printf("Channel: COS_N = %d\n", SIN_P);
    }
	}
}

uint16_t adc_read(uint8_t adc_channel){
	ADMUX &= 0xf0; // clear any previously used channel, but keep internal reference
	ADMUX |= adc_channel; // set the desired channel
	//start a conversion
	ADCSRA |= (1<<ADSC);
	// now wait for the conversion to complete
	while ( (ADCSRA & (1<<ADSC)) );
	// now we have the result, so we return it to the calling function as a 16 bit unsigned int
	return ADC;
}