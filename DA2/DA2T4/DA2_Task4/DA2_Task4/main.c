/*
 * DA2_Task4.c
 *
 * Created: 3/4/2016 9:54:58 AM
 * Author : Michael
 */ 

#define F_CPU 8000000UL		// define 8 MHz clock

#include <avr/io.h>			// include IO library
#include <util/delay.h>		// include library for _delay_ms function

void delay ();				// function declaration for delay

int main(void)
{
	DDRC |= (1 << PC0);		// set PC0 as output
	PORTC = 0;				// clear value on PC0
	while(1) {				// loop forever
		delay();			// call delay function for 250ms delay (.25 seconds)
		PORTC |= 0x01;		// set PC1 high
		delay();			// call delay function for 250ms delay (.25 seconds)
		PORTC = 0;			// set PC0 low
	}
	
}

void delay () {				// function body
	int i;					// declare i for counter
	for(i = 0; i < 25; i++) {	// loop 25 times
		_delay_ms(10);		//	delay 10 ms
	}						// total delay = 25*10ms = 250ms
}