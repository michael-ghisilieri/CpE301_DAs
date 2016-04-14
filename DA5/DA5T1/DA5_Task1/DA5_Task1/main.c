/*
 * DA5_Task1.c
 *
 * Created: 4/6/2016 4:32:27 PM
 * Author : Michael
 */ 

#define F_CPU 8000000UL //XTAL = 8MHZ

#include <avr/io.h> // library for I/O
#include <util/delay.h> // library for _delay_ms() function
#include <avr/sfr_defs.h> // library for loop_until_bit_is_clear function

void delay(unsigned short); // function to delay

int main()
{
	unsigned short potValue; // will hold value of pot. from ADC
	unsigned short period = 50; // total period of 50ms
	
	ADMUX |= (1 << REFS0); // AVcc with ext. cap. at AREF pin
	// ADCSRA will set prescaler to 8 and enable ADC
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
	
	DDRB = 0x01; // set PB0 as output
	PORTB = 0; // DC motor will always be on
	
	// the value used for stall ratios is 0.000975 which is actually
	// 1/1023 where 1023 is the max value of the 10-bit ADC
	// so the equation will be (value from pot. * 1/1023) or
	// (1 - (value from pot. * 1/1023)
	while(1)
	{
		ADCSRA |= (1 << ADSC); // start ADC conversion
		loop_until_bit_is_clear(ADCSRA, ADSC); // wait until ADC is done
		potValue = ADC; // store ADC value into potValue
		if (potValue == 0) // if potentiometer is turned completely off
			PORTB = 1; // turn DC motor off
		else {
			// these  two stalls will essentially be 75% on and (1 - 75%) = 25% if
			// the value of the potentiometer was turned 75% of the way
			PORTB = 0; // turn DC motor on
			delay((unsigned short)(period * (potValue*(.000975)))); // stall for ratio of pot.
			PORTB = 1; // turn DC motor off
			delay((unsigned short)(period * (1 - (potValue*(.000975))))); // stall for (1 - ratio of pot.)
		}
	}
	return 0 ;
}

void delay(unsigned short count) // count will be delay time in ms
{
	int i; // variable for for-loop iterations
	for(i = 0; i < count; i++)
	_delay_ms(1); // 1ms delay * count
}
