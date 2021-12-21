volatile unsigned long us_counter=0, total_time=0;

void delay_ms(unsigned int milliseconds){
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	// Set the value that you want to count to
	OCR0A = 0xF9;
	// start the timer
	reset_prescaler();
	//resetting pins
	TCCR0B |= (1 << CS01) | (1 << CS00);// set pre-scaler to 64 and start the timer
	for (int i=0; i<milliseconds; i++)
	{
		while ( (TIFR0 & (1 << OCF0A) ) == 0) // wait for the overflow event
		{
		}
		// reset the overflow flag
		TIFR0 = (1 << OCF0A);
	}
}
void delay_us(unsigned int microseconds){
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	// Set the value that you want to count to
	OCR0A = 0x0F;
	// start the timer
	reset_prescaler();
	TCCR0B |= (1 << CS00);// set pre-scaler to 1 and start the timer
	for (int i=0; i<microseconds; i++)
	{
		while ( (TIFR0 & (1 << OCF0A) ) == 0) // wait for the overflow event
		{
		}
		// reset the overflow flag
		TIFR0 = (1 << OCF0A);
	}
}
void init_getEcho (void){
	// Set the Timer Mode to CTC
	TCCR0A |= (1 << WGM01);
	// Set the value that you want to count to
	OCR0A = 0x5F;// 0-159 (160 timer ticks)
	//Start the timer
	TIMSK0 |= (1 << OCIE0A); //Set the ISR COMPA vect
	sei(); //enable interrupts;
	reset_prescaler(); //resetting pins
	TCCR0B |= (1 << CS00);// set pre-scaler to 1 and start the timer
}
ISR (TIMER0_COMPA_vect) {
	if (PINB & (1<<PINB0))
	{
		us_counter++;
	}
	else
	{
		if (us_counter !=0)
		{
			total_time = us_counter;
			us_counter = 0;
			disable_interrupt();
		}
	}
}
void reset_prescaler(void){
	TCCR0B &= ~((1<<CS00) | (1<<CS01));
}
void disable_interrupt(void){
	TCCR0B = 0;
}