#ifndef ULTRASOUND_SENSOR_H_INCLUDED
#define ULTRASOUND_SENSOR_H_INCLUDED

void delay_ms (unsigned int);
void delay_us (unsigned int);
void init_getEcho (void);
void reset_prescaler(void);
void disable_interrupt(void);

#endif