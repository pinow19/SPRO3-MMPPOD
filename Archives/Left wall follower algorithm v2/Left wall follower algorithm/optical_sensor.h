#ifndef OPTICAL_SENSOR_H_INCLUDED
#define OPTICAL_SENSOR_H_INCLUDED


#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef SLID_NR
#define SLID_NR 6
#endif
#ifndef Wheel_diameter
#define Wheel_diameter 6
#endif

extern volatile long travel_time,timer,timer_value_holder,timer_between_two_slids,t;
extern volatile float avg_slids_per_min,avg_rpm;
extern volatile int nr_slids,distance,flag1,flag2,nr_slids_before;

void milisec_delay(void);
void interrupt_enabler(void);
void Pinsetup(void);
void slid_counter(void);
void slids_per_min(void);
void show_values(void);

#endif