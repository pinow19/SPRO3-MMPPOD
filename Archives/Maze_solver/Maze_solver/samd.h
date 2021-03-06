#ifndef SAMD_H
#define SAMD_H

//Enumeration
enum Motor {ENA = 5, //D5 -> Left motor
            ENB = 6, //D6 -> Right motor
            IN1 = 4, //D4 -> Left motor
            IN2 = 7, //D7 -> Left motor
            IN3 = 0, //D8 -> Right motor
            IN4 = 1}; //D9 -> Right motor
enum Motor_Directions {FORWARD, BACKWARD, LEFT, RIGHT, STOP};
enum Ultrasonic {U_FRONT = 0, //A0 and D10
                 U_LEFT = 1, //A1 and D11
                 U_RIGHT = 2}; //A2 and D12
enum Optocoupler {O_LEFT = 2, //D2 -> LEFT
                  O_RIGHT = 3}; //D3 -> RIGHT
enum Distance_Allowances {OUT_OF_RANGE_HIGH = 150,
						  OUT_OF_RANGE_LOW = 2};
enum Analog_Sensors{TEMP_SENSOR = 3, //A3
					VOLTAGE_SENSOR = 4}; //A4

//Functions
unsigned int ultrasound_sensor (unsigned char);
void delay_ms (unsigned int);
void delay_us (unsigned int);
void set_pins (void);
void select_motor_direction (unsigned char, unsigned char);
unsigned int analog_sensor (uint8_t);
void optocoupler_slids (unsigned char);
void reset_slid_nr_and_stop_interrupt(void);

//External slids
extern volatile unsigned char slid_nr;


#endif