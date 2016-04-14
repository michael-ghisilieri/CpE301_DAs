/*
 * DA5_Task3.c
 *
 * Created: 4/7/2016 6:15:26 PM
 * Author : Michael
 */ 

// brown = ground
// red = power
// orange = PWM

#define F_CPU 8000000UL // clock frequency for delay

#include <util/delay.h> // library for _delay_ms() function
#include <avr/io.h> // library for I/O
#include <avr/sfr_defs.h> // library for loop_until_bit_is_clear function

void delay (); // function declaration for delay

int main()
{
	unsigned short potValue; // will hold value of pot. from ADC
	unsigned short angle = 0; // angle of servo motor
	
	ADMUX |= (1 << REFS0); // AVcc with ext. cap. at AREF pin
	// ADCSRA will set prescaler to 8 and enable ADC
	ADCSRA |= (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
	
	DDRB = 0x02; // PB1/OC1A as output to PWM of servo
	//ICR1 = 20000 defines 50Hz PWM
	ICR1 = 20000; // value that timer will catch on interrupt
	// TCCR1A will toggle on compare match, Fast PWM with ICR1 as top value
	TCCR1A|=(0<<COM1A0)|(1<<COM1A1)|(0<<COM1B0)|(0<<COM1B1)|(1<<WGM11)|(0<<WGM10);
	// prescaler of 8
	TCCR1B|=(0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
	
	// the value used for angle ratio is 0.000975 which is actually
	// 1/1023 where 1023 is the max value of the 10-bit ADC
	// so the equation will be (1 - (value from pot. * 1/1023))
	// to match the way at which this specific servo rotates
	while(1)
	{
		ADCSRA |= (1 << ADSC); // start ADC conversion
		loop_until_bit_is_clear(ADCSRA, ADSC); // wait until ADC is done
		potValue = ADC; // store ADC value into potValue
		
		// Since the range from 0 degrees to 180 degrees is the
		// max value minus the minimum, it will be 2300 - 500 = 1800
		// multiple the range by the potentiometer ratio
		angle = 1800*(1 - (potValue*(.000975)));
		angle += 500; // add 500 since 0 degrees is at 500
		OCR1A = angle; // set OCR1A to calculated value based on pot.
		delay(); // delay for 1 second
	}
}

void delay () {
	int i;					// declare i for counter
	for(i = 0; i < 100; i++) {	// loop 100 times
		_delay_ms(10);		//	delay 10 ms
	}						// total delay = 100*10ms = 1000ms
}
