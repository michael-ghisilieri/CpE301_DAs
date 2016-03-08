;
; DA2_Task1.asm
;
; Created: 3/1/2016 7:01:20 PM
; Author : Michael
;

; Macro that initializes stack pointer
.MACRO INITSTACK
	LDI	R16, HIGH(RAMEND)	
	OUT SPH, R16	
	LDI R16, LOW(RAMEND)	
	OUT SPL, R16	
.ENDMACRO

	SBI DDRC, 0			; set PC0 as an output
	LDI R16, 0x00		; load zero into R16
	OUT DDRC, R16		; clear PC0
Counter:
	CALL Delay			; call delay for 0.25 seconds
	LDI  R22, 0x01		; load one into R22
	OUT PORTC, R22		; set PC0 high
	CALL Delay			; call delay for 0.25 seconds
	LDI  R22, 0x00		; load zero into R22
	OUT PORTC, R22		; set PC0 low
	JMP Counter			; loop back 


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