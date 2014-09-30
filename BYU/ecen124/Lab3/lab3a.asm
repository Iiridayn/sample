;; Name: Michael Clark
;; Grader: Scott
;; Assignment: lab3
;;
;; Adds the user input until a ! is received
;;
.ORIG 	x3000		; Initial addy
	AND R1, R1, #0	; Clear R1
	LD R3, BANG	; Setup for '!' check
	LD R4, NUM	; Setup for ASCII->Num
LOOP 	TRAP x20	; Get char
	ADD R2, R0, R3	; Is 0 if '!'
	BRz END		; Done
	ADD R2, R0, R4	; Get # value of char
	ADD R1, R1, R2	; R1 += R2
	BRnzp LOOP	; Go back to Trap call
END 	TRAP x25	; HALT
BANG	.FILL	xFFDF	; Negated char value of '!'
NUM	.FILL	xFFD0	; Negated char value of '0'