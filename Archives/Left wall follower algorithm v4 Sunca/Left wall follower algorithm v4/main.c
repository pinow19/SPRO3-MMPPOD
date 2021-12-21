/*
 * Test.c
 *
 * Created: 03/12/2021 01.12.44
 * Author : danie
 */ 

#include <avr/io.h>
#include "samd.h"
#include <stdio.h>
#include "usart.h"

volatile int state = 0;

int main(void)
{
	uart_init();
	io_redirect();
	set_pins();
	unsigned int FrontSensor = 0, RightSensor = 0, LeftSensor = 0;

// printf("hello \n");



     while (1) //if all three sensors does not sense anything
	{	  
			  //  printf("hello \n");
			  FrontSensor = ultrasound_sensor(U_FRONT);
			  LeftSensor = ultrasound_sensor(U_LEFT);
			  RightSensor = ultrasound_sensor(U_RIGHT);
			  
			//  printf("%d\n", state);
			  
			  if (FrontSensor < DIS && RightSensor < DIS && LeftSensor < DIS) //obstacles in front of all three sides
			  {
				//  state = 1;
				 select_motor_direction(STOP);
				 delay_ms(500);
				  while(!(FrontSensor > DIS))
				  {
					 
					  select_motor_direction(RIGHT); //Turn right until there is free way at front
					  delay_ms(800);
					  select_motor_direction(STOP);
					  delay_ms(100);
					  FrontSensor = ultrasound_sensor(U_FRONT);
					  LeftSensor = ultrasound_sensor(U_LEFT);
					  RightSensor = ultrasound_sensor(U_RIGHT);
					 
				  }
			  }
			  
			  else if (FrontSensor < DIS && RightSensor < DIS && LeftSensor > DIS) //obstacles on right and front sides
			  {
				  //state = 2;
				  select_motor_direction(STOP);
				  delay_ms(500);
				  while(!(FrontSensor > DIS))
				  				  {
					  				  select_motor_direction(LEFT); //turn left side
					  				  delay_ms(800);
					  				  select_motor_direction(STOP);
					  				  delay_ms(100);
					  				  FrontSensor = ultrasound_sensor(U_FRONT);
					  				  LeftSensor = ultrasound_sensor(U_LEFT);
					  				  RightSensor = ultrasound_sensor(U_RIGHT);
				  				  }
				 
			  }
			  
			  else if (FrontSensor < DIS && RightSensor > DIS && LeftSensor < DIS) //obstacles on left and front sides
			  {
				  
				  select_motor_direction(STOP);
				  delay_ms(500);
				  while(!(FrontSensor > DIS))
				  				  {
					  				  select_motor_direction(RIGHT); //turn right side
					  				  delay_ms(800);
					  				  select_motor_direction(STOP);
					  				  delay_ms(100);
					  				  FrontSensor = ultrasound_sensor(U_FRONT);
					  				  LeftSensor = ultrasound_sensor(U_LEFT);
					  				  RightSensor = ultrasound_sensor(U_RIGHT);
				  				  }
				 
			  }
			  
			  else if (FrontSensor < DIS && RightSensor > DIS && LeftSensor > DIS) //obstacles on the front side
			  {
				  //state = 4;
				  select_motor_direction(STOP);
				  delay_ms(500);
				  while(!(FrontSensor > DIS))
				  				  {
					  				  select_motor_direction(RIGHT); //turn right side
					  				  delay_ms(800);
									  select_motor_direction(STOP);
									  delay_ms(100);
					  				  FrontSensor = ultrasound_sensor(U_FRONT);
					  				  LeftSensor = ultrasound_sensor(U_LEFT);
					  				  RightSensor = ultrasound_sensor(U_RIGHT);
					  				  //select_motor_direction(FORWARD);
				  				  }
			  }
			  /*else if (FrontSensor > DIS && RightSensor < DIS && LeftSensor > DIS) //obstacle on the right side
			  {
				 // state = 5;
				 select_motor_direction(STOP);
				 delay_ms(500);
				  select_motor_direction (FORWARD); //turn right side
				  delay_ms(300);
				  FrontSensor = ultrasound_sensor(U_FRONT);
				  LeftSensor = ultrasound_sensor(U_LEFT);
				  RightSensor = ultrasound_sensor(U_RIGHT);
				//  select_motor_direction(FORWARD);
			  }
			  */
			  else
			  {
				 // state = 0;
				  select_motor_direction(FORWARD);
				//  delay_ms(1000);
			  }
			  
			  //  printf("Left sensor %d \n",LeftSensor);
			  
		  }
  
  //select_motor_direction(STOP); //Stop motor	
    
}