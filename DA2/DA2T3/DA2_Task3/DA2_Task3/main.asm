;
; DA2_Task3.asm
;
; Created: 3/3/2016 4:20:18 PM
; Author : Michael
;

	.ORG 0x00			; set program counter to 0x00
	RJMP INIT			; jump to INIT label skipping by interrupt at 0x16
	.ORG 0x16			; location of TIM1_COMPA in interrupt table
	RJMP TIM1_COMPA		; jump to TIM1_COMPA every time interrupt occurs

INIT:
; Macro that initializes stack pointer
.MACRO INITSTACK
	LDI	R16, HIGH(RAMEND)	
	OUT SPH, R16	
	LDI R16, LOW(RAMEND)	
	OUT SPL, R16	
.ENDMACRO
	
	INITSTACK
	SBI DDRB, 0
	SBI DDRB, 1
	SBI DDRB, 2
	SBI DDRB, 3
	SBI DDRB, 4
	SBI DDRB, 5
	SBI DDRB, 6
	SBI DDRB, 7			; set every pin of PORTB as an output for bargraph
	SBI DDRC, 0
	SBI DDRC, 5
	SBI DDRC, 4			; set pins 5 and 6 as outputs for bargraph
	LDI R16, 0x00
	OUT PORTB, R16		; turn off all LEDs on bargraph
	OUT PORTC, R16		; turn off all LEDs on bargraph
	LDI R20, 0x00		; initialize counter to zero
	LDI R21, 0x00		; second counter to check if value is 5 or 10
Begin:
	LDI R24, 0x0F		; load 0x0F into R24
	STS OCR1AH, R24		; store 0x0F into the high register of OCR1A
	LDI R24, 0x42		; load 0x42 into R24
	STS OCR1AL, R24		; store 0x42 into the low register of OCR1A
	LDI R24, 0x00		; load 0x00 into R24
	STS TCCR1A, R24		; set CTC and have OC1A be disconnected to not change bits on bargraph
	LDI R24, 0x0D		; load 0x0D into R24
	STS TCCR1B, R24		; set prescaler to 1024 and WGM12 bit high
	LDI R24, 0x02		; load 0x02 into R24
	STS TIMSK1, R24		; set OCIEA1 high to enable interrupts for OCF1A
	SEI					; enable global interrupts
Forever:
	RJMP Forever		; loop forever and wait for interrupt to occur every 0.5 second


TIM1_COMPA:
	INC R20				; increment counter
	OUT PORTB, R20		; update value of counter on LED bargraph
	INC R21				; increment register checking for 5th/10th rising edge
; the two extra LEDs will always be in 1 out of 4 different states	
	CPI R21, 5			; if first 5th pulse
	BREQ Fifth			; branch and set accordingly
	CPI R21, 10			; if first 10th pulse
	BREQ Tenth			; branch and set accordingly
	CPI R21, 15			; if second 5th pulse
	BREQ Fifteenth		; branch and set accordingly
	CPI R21, 20			; if second 10th pulse
	BREQ Twentieth		; branch and set accordingly
	JMP ToggleDone		; don't update LEDs and jump to negative edge delay
Fifth:
	LDI R16, 0x20
	OUT PORTC, R16		; first 5th pulse where PC4 = 0 and PC5 = 1
	JMP ToggleDone		; jump to negative edge delay
Tenth:
	LDI R16, 0x10
	OUT PORTC, R16		; first 10th pulse where PC4 = 1 and PC5 = 0
	JMP ToggleDone		; jump to negative edge delay
Fifteenth:
	LDI R16, 0x30
	OUT PORTC, R16		; second 5th pulse where PC4 = 1 and PC5 = 1
	JMP ToggleDone		; jump to negative edge delay
Twentieth:
	LDI R16, 0x00
	OUT PORTC, R16		; second 10th pulse where PC4 = 0 and PC5 = 0
	LDI R21, 0x00		; set counter back to zero for checks
	JMP ToggleDone		; jump to negative edge delay
ToggleDone:
	LDI R24, 0x02		; load 0x02 into R24
	STS TIFR1, R24		; clear OCF1A flag
	RETI
