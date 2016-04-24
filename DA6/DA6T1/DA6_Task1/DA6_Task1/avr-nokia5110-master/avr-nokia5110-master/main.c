/*
 * DA6_Task1.c
 *
 * Created: 4/21/2016 5:53:10 PM
 * Author : Michael
 */ 

#define F_CPU 8000000UL // declare clock frequency as 8MHz for delay function

#include <avr/io.h> // library for I/O
#include <stdint.h> // needed for uintX_t variables
#include <avr/interrupt.h> // library for ADC interrupt
#include <util/delay.h> // library for _delay_ms() function

#include "nokia5110.h" // nokia 5110 file for its functions 

void delay (); // function declaration for delay

// 32 bit integer to hold ADC value during conversion to Fahrenheit value
volatile uint32_t temperature;
volatile uint16_t ADCvalue; // Global variable, set to volatile if used with ISR
volatile char temp_out; // current digit of temperature being transmitted

int main(void)
{	
	nokia_lcd_init(); // initialize the LCD
	nokia_lcd_clear(); // clear the LCD
	
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

	return 0;
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
	nokia_lcd_set_cursor(0, 0); // set cursor to beginning of LCD
	nokia_lcd_write_string("Fahrenheit", 1); // print "Fahrenheit" with size scale of 1
	nokia_lcd_set_cursor(0, 10); // move to second line
	nokia_lcd_write_char(temp_out+'0', 4); // print tens digit with size scale of 4
	temp_out = temperature % 10; // % 10 will give the ones digit of a 2 digit temp
	nokia_lcd_write_char(temp_out+'0', 4); // print ones digit with size scale of 4
	nokia_lcd_render(); // render to display all the writes from above
	delay(); // delay for 1000 ms or 1 second
}

void delay () {
	int i;					// declare i for counter
	for(i = 0; i < 100; i++) {	// loop 100 times
		_delay_ms(10);		//	delay 10 ms
	}						// total delay = 100*10ms = 1000ms
}
