;
; DA0_Task1.asm
;
; Created: 2/13/2016 4:27:15 AM
; Author : Michael
;


	.org	0				; assemble code at 0x0000
	sbi		DDRB, 2			; use PORT
	ldi		R20, 0			; intialize register used as sum to zero
	ldi		R21, 37			; load 37 into register R21 (first random number)
	add		R20, R21		; R20 = R20 + R21 (add first number to sum)
	ldi		R21, 59			; load 59 into register R21 (second random number)
	add		R20, R21		; R20 = R20 + R21 (add second number to sum)
	ldi		R21, 52			; load 42 into register R21 (third random number)
	add		R20, R21		; R20 = R20 + R21 (add third number to sum)
	ldi		R21, 58			; load 48 into register R21 (fourth random number)
	add		R20, R21		; R20 = R20 + R21 (add fourth number to sum)
	ldi		R21, 55			; load 35 into register R21 (fifth random number)
	add		R20, R21		; R20 = R20 + R21 (add fifth number to sum)
	brcs	overflow		; if there is an overflow (carry flag set), branch to overflow label
	ldi		R21, 0			; load 0 into register R21
	out		PORTB, R21		; zero out PORTB so that pin 2 = LOW
	jmp		done			; if there wasn't an overflow, jump to done since PORTB has already been set
overflow:
	ldi		R21, 4			; load 4 into register R21
	out		PORTB, R21		; set PORTB to 4 so that pin 2 = HIGH
done:
	jmp		done			; infinite loop once program is finished running