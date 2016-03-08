/*
 * DA2_Task5.c
 *
 * Created: 3/4/2016 10:48:55 AM
 * Author : Michael
 */ 

#define F_CPU 8000000UL		// define 8 MHz clock

#include <avr/io.h>			// include IO library
#include <util/delay.h>		// include library for _delay_ms function

void delay ();				// function declaration for delay

int main(void)
{
	int counter = 0;		// set counter for LED bargraph starting at 0
	int multiple = 0;		// set counter for 5th/10th rising edge starting at 0
	DDRB = 0xFF;			// set every pin on PORTB as an output
	DDRC = 0x30;			// set PC4 an PC5 as outputs on PORTC
	while(1) {				// loop forever
		delay();			// call delay function for 250ms delay (.25 seconds)
		counter++;			// increment counter after delay
		if(counter > 255)	// if counter went above 255 (9th bit)
			counter = 0;	// reset counter back to zero
		PORTB = counter;	// output counter to LED bargraph
		multiple++;			// increment counter for 5th/10th checks
		switch(multiple) {  
			case 5:			// if first 5th rising edge
				PORTC = 0x20;	// PC4 = 0 and PC5 = 1
				break;
			case 10:		// if first 10th rising edge
				PORTC = 0x10;	// PC4 = 1 and PC5 = 0
				break;
			case 15:		// if second 5th rising edge
				PORTC = 0x30;	// PC4 = 1 and PC5 = 1
				break;
			case 20:		// if second 10th rising edge
				PORTC = 0x00;	// PC4 = 0 and PC5 = 0
				multiple = 0;	// clear check counter
				break;
			default:
				break;
		}
		delay();			// call delay function for 250ms delay (.25 seconds)
	}
	
}

void delay () {				// function body
	int i;					// declare i for counter
	for(i = 0; i < 25; i++) {	// loop 25 times
		_delay_ms(10);		//	delay 10 ms
	}						// total delay = 25*10ms = 250ms
}