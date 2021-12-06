/*
 * Test.c
 *
 * Created: 03/12/2021 01.12.44
 * Author : danie
 */ 

#include <avr/io.h>
#include "samd.h"
#include <stdio.h>

int main(void)
{
	set_pins();
	unsigned int FrontSensor = 0, RightSensor = 0, LeftSensor = 0;
  do
  {
	  FrontSensor = ultrasound_sensor(U_FRONT);
	  LeftSensor = ultrasound_sensor(U_LEFT);
	  RightSensor = ultrasound_sensor(U_RIGHT);

	  if (FrontSensor < DIS && RightSensor < DIS && LeftSensor < DIS) //obstacles in front of all three sides
	  {
		  while(!(FrontSensor > DIS)){
			  select_motor_direction(RIGHT); //Turn right until there is free way at front
			  FrontSensor = ultrasound_sensor(U_FRONT);
		  }
	  }

	  else if (FrontSensor < DIS && RightSensor < DIS && LeftSensor > DIS) //obstacles on right and front sides
	  {
		  select_motor_direction(LEFT); //turn left side
	  }

	  else if (FrontSensor < DIS && RightSensor > DIS && LeftSensor < DIS) //obstacles on left and front sides
	  {
		  select_motor_direction(RIGHT); //turn right side
	  }
	  
	  else if (FrontSensor < DIS && RightSensor > DIS && LeftSensor > DIS) //obstacles on the front side
	  {
		  select_motor_direction(RIGHT); //turn right side
		  delay_ms(180);
		  select_motor_direction(FORWARD);
	  }
	  else if (FrontSensor > DIS && RightSensor < DIS && LeftSensor > DIS) //obstacle on the right side
	  {
		  select_motor_direction (LEFT); //turn right side
		  delay_ms(180);
		  select_motor_direction(FORWARD);
	  }

	  else
	  {
		  select_motor_direction(FORWARD);
	  }
	  
  } while (!(FrontSensor > DIS && RightSensor > DIS && LeftSensor > DIS)); //if all three sensors does not sense anything
  select_motor_direction(STOP); //Stop motor	
}

