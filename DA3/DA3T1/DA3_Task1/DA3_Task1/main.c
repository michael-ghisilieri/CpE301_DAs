/*
 * DA3_Task1.c
 *
 * Created: 3/17/2016 2:42:22 PM
 * Author : Michael
 */ 

// This code waits for a character and transmits the character back (with interrupts)

#define F_CPU 8000000UL // Clock Speed

#include <avr/io.h>
#include <stdint.h> // needed for uint8_t
#include <avr/interrupt.h>
#include <util/delay.h>		// include library for _delay_ms function

void USART0SendByte(char);
void delay ();				// function declaration for delay

// 32 bit integer to hold ADC value during conversion to Fahrenheit value
volatile uint32_t temperature;
volatile uint16_t ADCvalue; // Global variable, set to volatile if used with ISR
volatile char temp_out; // current digit of temperature being transmitted

int main(void)
{
	// UBRR = (F_CPU / (16 * 9600)) - 1
	// UBRR = (8 MHz / (16 * 9600)) - 1 = 51 or 0x33
	UBRR0L = 0x33; // set based on equation using baud rate of 9600

	UCSR0B	= (1 << TXEN0); // Enable transmitter
	UCSR0C	= (1 << UCSZ01) | (1 << UCSZ00); // 8 bit data
	
	ADMUX	= 0; // use ADC0
	ADMUX	|= (1 << REFS0); // use AVcc as the reference
	ADCSRA	|= (1 << ADPS2) | (1 << ADPS1); // 64 prescale for 16Mhz
	ADCSRA	|= (1 << ADATE); // Set ADC Auto Trigger Enable
	ADCSRB	= 0; // 0 for free running mode
	ADCSRA	|= (1 << ADEN); // Enable the ADC
	ADCSRA	|= (1 << ADIE); // Enable Interrupts
	ADCSRA	|= (1 << ADSC); // Start the ADC conversion
	sei();

	while(1) // loop forever and wait for ADC interrupt
	{
	}
}

ISR(ADC_vect)
{
	ADCvalue = ADC; // read all bits of ADC (10) and store into ADCvalue
	temperature = ADCvalue; // copy value into new variable in case ADC overwrites it
	// equation used to convert ADC value is:
	// (ADC register * 5 * 100) / 1024
	// which is the ADC multiplied by the reference voltage multiplied by 100
	// since the LM34 sensor gives its value in mV and then divide by 1024 since
	// a ten bit register will have a max value of 2^10 or 1024
	temperature = temperature*5;
	temperature = temperature*100;
	temperature = temperature/1024;
	temp_out = temperature / 10; // dividing 2 digit temp (i.e. 75 or 80) will give tens digit
	USART0SendByte((temp_out+'0')); // output tens digit converted to its matching char
	temp_out = temperature % 10; // % 10 will give the ones digit of a 2 digit temp
	USART0SendByte((temp_out+'0')); // output ones digit converted to its matching char
	USART0SendByte('\n'); // send a line feed to the terminal
	USART0SendByte('\r'); // send return to the terminal to move cursor of terminal
	delay(); // delay for 1000 ms or 1 second
}

void USART0SendByte(char u8Data)
{
	//wait while previous byte is completed
	while(!(UCSR0A&(1<<UDRE0))){};

	// Transmit data
	UDR0 = u8Data;
}

void delay () {
	int i;					// declare i for counter
	for(i = 0; i < 100; i++) {	// loop 100 times
		_delay_ms(10);		//	delay 10 ms
	}						// total delay = 100*10ms = 1000ms
}
