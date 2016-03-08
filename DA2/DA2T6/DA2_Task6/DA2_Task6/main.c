/*
 * DA2_Task6.c
 *
 * Created: 3/4/2016 11:10:02 AM
 * Author : Michael
 */ 

#include <avr/io.h>			// include IO library
#include <avr/interrupt.h>	// include library for interrupts

#define F_CPU 8000000UL		// define 8 MHz clock

int counter;				// global variable for counter
int multiple;				// global variable for check counter (5th/10th)

ISR(TIMER1_COMPA_vect)		// interrupt subroutine body
{
	counter++;				// increment counter for LED bargraph
	if(counter > 255)		// if counter went above 255 (9th bit)
		counter = 0;		// reset counter back to zero
	PORTB = counter;		// output counter to LED bargraph
	multiple++;				// increment counter for 5th/10th checks
	switch(multiple) {
		case 5:				// if first 5th rising edge
			PORTC = 0x20;	// PC4 = 0 and PC5 = 1
			break;
		case 10:			// if first 10th rising edge
			PORTC = 0x10;	// PC4 = 1 and PC5 = 0
			break;
		case 15:			// if second 5th rising edge
			PORTC = 0x30;	// PC4 = 1 and PC5 = 1
			break;
		case 20:			// if second 10th rising edge
			PORTC = 0x00;	// PC4 = 0 and PC5 = 0
			multiple = 0;	// clear check counter
			break;
		default:
			break;
	}
}

int main(void)
{
	counter = 0;		// initialize counter to zero
	multiple = 0;		// initialize check counter to zero
	DDRB = 0xFF;		// set all of PORTB as outputs
	DDRC = 0x30;		// set PC4 and PC5 as outputs
	TCCR1B |= 0x0D;		// setup up CTC mode and prescaler (1024) for timer
	TCNT1 = 0;			// initialize timer1 to zero
	OCR1A = 3906;		// set CTC value
	TIMSK1 |= (1 << OCIE1A);	//enable compare interrupt
	sei();				// enable global interrupts
	
    while (1)			// loop forever and execute interrupt
    {					// subroutine every 0.5 seconds
    }
}

