#define CW 1
#define CCW 2
#define BRAKE 3
#define number_of_motors 2

struct Motors_t{
	unsigned int pwm_value;
	int direction;
	int state[2];
};

struct Motors_t M[number_of_motors];

void setup() {
	DDRD |= (1<<DDD5)|(1<<DDD6); //PWM signals
	DDRD |= (1<<DDD2)|(1<<DDD4); //Direction for motor 1
	DDRB |= (1<<DDB0)|(1<<DDB4); //Direction for motor 2
	
	TCCR0A = (1<<WGM01)|(1<<WGM00); //fast pwm mode
	TCCR0A|= ((1<<COM0A1) | (1<<COM0B1)); //non-inverting something
	
	TCCR0B = (1<<CS22); //prescaler = 64
}
