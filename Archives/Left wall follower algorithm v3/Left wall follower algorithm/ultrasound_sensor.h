#ifndef ULTRASOUND_SENSOR_H_INCLUDED
#define ULTRASOUND_SENSOR_H_INCLUDED

extern volatile unsigned long us_counter, total_time;

unsigned int ultra_sound_sensor(unsigned int sensor);
void delay_ms (unsigned int);
void delay_us (unsigned int);
void init_getEcho (void);
void reset_prescaler(void);
void disable_interrupt(void);

#endif
