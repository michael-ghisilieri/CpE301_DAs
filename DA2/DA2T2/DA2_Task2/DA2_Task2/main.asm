;
; DA2_Task2.asm
;
; Created: 3/1/2016 1:50:09 PM
; Author : Michael
;

; Macro that initializes stack pointer
.MACRO INITSTACK
	LDI	R16, HIGH(RAMEND)	
	OUT SPH, R16	
	LDI R16, LOW(RAMEND)	
	OUT SPL, R16	
.ENDMACRO

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
	SBI DDRC, 4			; set pins 5 and 4 as outputs for bargraph
	LDI R16, 0x00
	OUT PORTB, R16		; turn off all LEDs on bargraph
	OUT PORTC, R16		; turn off all LEDs on bargraph
	LDI R20, 0x00		; initialize counter to zero
	LDI R21, 0x00		; second counter to check if value is 5 or 10
Counter:
	CALL Delay			; delay for 0.25 seconds
	INC R20				; increment counter
	OUT PORTB, R20		; update value of counter on LED bargraph
	INC R21				; increment register checking for 5th/10th rising edge
; the two extra LEDs (9 and 10) will always be in 1 out of 4 different states	
	CPI R21, 5			; if first 5th pulse
	BREQ Fifth			; branch and set accordingly
	CPI R21, 10			; if first 10th pulse
	BREQ Tenth			; branch and set accordingly
	CPI R21, 15			; if second 5th pulse
	BREQ Fifteenth		; branch and set accordingly
	CPI R21, 20			; if second 10th pulse
	BREQ Twentieth		; branch and set accordingly
	JMP NegEdge			; if not 5th or 10th pulse don't update LEDs and jump to NegEdge label
; since PC6 was a reset pin I used PC4 instead
; so PC4 is used as the 10th LED and PC5 is still 9th
Fifth:
	LDI R16, 0x20
	OUT PORTC, R16		; first 5th pulse where PC4 = 0 and PC5 = 1
	JMP NegEdge			; jump to negative edge delay
Tenth:
	LDI R16, 0x10
	OUT PORTC, R16		; first 10th pulse where PC4 = 1 and P4C5 = 0
	JMP NegEdge			; jump to negative edge delay
Fifteenth:
	LDI R16, 0x30 ;0x60
	OUT PORTC, R16		; second 5th pulse where PC4 = 1 and PC5 = 1
	JMP NegEdge			; jump to negative edge delay
Twentieth:
	LDI R16, 0x00 ;0x00
	OUT PORTC, R16		; second 10th pulse where PC4 = 0 and PC5 = 0
	LDI R21, 0x00		; set counter back to zero for checks
	JMP NegEdge			; jump to negative edge delay
NegEdge:
	CALL Delay			; delay for 0.25 seconds
	JMP Counter

; based on simulation
; total delay will be 1_999_945 cycles at 8 MHz so 1_999_945/8_000_000 = .24999 seconds
; value calculated by hand was a slightly larger than 2_000_000 but still very close to
; the total delay and the total delay is actually closer than the calculated value
Delay:
	PUSH R16
	PUSH R17
	PUSH R18			; push registers used in function call
	LDI R16, 25			; outer loop value loaded 1 time
L1:						; 3*25 = 75 without extra branch cycles
	LDI R17, 94			; middle loop value loaded 25 times
L2:						; 3*94*25 = 7_050 without extra branch cycles
	LDI R18, 212		; inner loop value loaded 2350 times
L3:						; 4*94*212*25 = 1_992_800 without extra branch cycles
	NOP
	DEC R18
	BRNE L3	
	DEC R17
	BRNE L2
	DEC R16
	BRNE L1
	
	POP R18
	POP R17
	POP R16				; pop registers used in function call
	RET					; return back to counter loop