#ifndef MOTOR_BOARD_CONTROLLER_H_INCLUDED
#define MOTOR_BOARD_CONTROLLER_H_INCLUDED

#ifndef CW
#define CW 1
#endif
#ifndef CCW
#define CCW 2
#endif
#ifndef BRAKE
#define BRAKE 3
#endif
#ifndef number_of_motors
#define number_of_motors 2
#endif

struct Motors_t{
	unsigned int pwm_value;
	int direction;
	int state[2];
};
struct Motors_t M[number_of_motors];

void setup(void);
void motor_setup(void);
#endif
