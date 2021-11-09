#ifndef DS1621_H_INCLUDED
#define DS1621_H_INCLUDED

//DS1621 address (a2 = a1 = a0 = 0)
#define DS1621_adr 0x90

//function headers
double get_temperature(void);
void ds1621_init(void);

#endif


