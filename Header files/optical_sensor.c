#define SLID_NR 6
#define Wheel_diameter 6

volatile long  travel_time=0,timer=0,timer_value_holder=0,timer_between_two_slids=0,t=0;
volatile float avg_slids_per_min=0,avg_rpm=0;
volatile int nr_slids=0,distance=0,flag1=0,flag2=0,nr_slids_before=0;

void Pinsetup(void)
{
	DDRD &= ~(1<<DDD3);// PORTD3 becomes an input pin
	PORTD &=~(1<<DDD3);
}

void milisec_delay(void)
{
	TCCR0A |= (1<<WGM01);// sets timer to CTC mode
	
	OCR0A = 0xF9;// this value will asure a 1 milisecond delay for a prescalar of 64
	
	TIMSK0|= (1<<OCIE0A);// this line enables the timer0 Compare Match A Interrupt
	
	TCCR0B |= (1<<CS01) | (1<<CS00);// this line sets the value for the prescaler as 64 and it also starts the timer
	
}

void interrupt_enabler(void)
{
	EICRA |= (1<<ISC11);// only the falling edge will generate an interrupt request on the interrupt
	
	EICRA &= ~(1<<ISC10);
	
	EIMSK |= (1<<INT1);// enables external interrupt number 1 (PD3)
	
	EIMSK &= ~(1<<INT0);
	
	sei();
}

void slid_counter(void)
{
	if( (( PIND &= (0<<DDD3) )==0) && (flag1==1))
	{
		nr_slids_before=nr_slids;
		timer_value_holder=travel_time;
		nr_slids++;
		flag1=0;
	}
}

void slids_per_min(void)
{
	timer=travel_time;
	if( (( PIND &= (0<<DDD3) )==0) && (flag2==1) )
	{
		t=timer_between_two_slids;
		flag2=0;
		
		avg_slids_per_min=(1000*60.0)/timer_between_two_slids; //average slids/min
		
	}
}

void show_values()
{
	if(x==1) x=2;
	if(x==0){
	printf("The total travel time is %ld ms\n",travel_time);
	printf("The number of slids detected is %d \n",nr_slids);
	//printf("timer between slids is %ld and t is %ld \n",timer_between_two_slids,t);
//	printf("The value that is stored in timer value holder is %ld \n",timer_value_holder);
	// printf("Distance traveled in cm is %d and the average nr of slids/min is %0.2f\n",distance,(float)avg_slids_per_min);
	//printf("The average rpm is %0.2f rotations/min \n",avg_rpm);
	x=1;
	}
	if(x==2) x=0;
}

ISR (INT1_vect)// external interrupt 1
{
	while( ( PIND &= (1<<DDD3) ) == 0 )
	{
		sei();
		
		milisec_delay();
		flag1=1;
		flag2=1;
	}

 
	
}


ISR (TIMER0_COMPA_vect) // timer0 overflow interrupt
{
	sei();
	travel_time++;
	timer_between_two_slids=travel_time-timer_value_holder;
}