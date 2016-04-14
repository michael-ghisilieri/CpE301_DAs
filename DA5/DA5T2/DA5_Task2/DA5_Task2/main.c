/*
 * DA5_Task2.c
 *
 * Created: 4/7/2016 5:15:29 PM
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
	
	DDRB = 0xFF; // set all of PORTB as output
	
	// the value used for stall ratios is 0.000975 which is actually
	// 1/1023 where 1023 is the max value of the 10-bit ADC
	// so the equation will be (value from pot. * 1/1023) or
	// (1 - (value from pot. * 1/1023)
	while(1)
	{
		// black, red, yellow, blue
		ADCSRA |= (1 << ADSC); // start ADC conversion
		loop_until_bit_is_clear(ADCSRA, ADSC); // wait until ADC is done
		potValue = ADC; // store ADC value into potValue

		// green, red, black, blue
		if (potValue == 0) // if potentiometer is turned completely off
			// stepper will not move and stay in place
			PORTB = PORTB;
			// These values being output to PORTB will cause
			// the coils to spin the motor in a clockwise direction
			// 0x06 = 0110 which will mean give the wires
			// green = 0, red = 1, black = 1, blue = 0
		else {
			// Since turning the potentiometer to a higher value, it should
			// cause the speed to increase which will mean a smaller delay
			// with the function delay(). This is why I delay with the
			// equation (1-ratio) since a higher value will delay for less
			// result in a faster speed.
			PORTB = 0x06;
			delay((unsigned short)(period * (1- (potValue*(.000975)))));
			PORTB = 0x0C;
			delay((unsigned short)(period * (1- (potValue*(.000975)))));
			PORTB = 0x09;
			delay((unsigned short)(period * (1- (potValue*(.000975)))));
			PORTB = 0x03;
			delay((unsigned short)(period * (1- (potValue*(.000975)))));
		}
	}
	return 0;
}

void delay(unsigned short count) // count will be delay time in ms
{
	int i; // variable for for-loop iterations
	for(i = 0; i < count; i++)
	_delay_ms(1); // 1ms delay * count
}
