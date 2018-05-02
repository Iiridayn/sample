;; Name: Michael Clark
;; Grader: Scott
;; Assignment: lab5
;;
;; Gets two binary numbers from the user and preforms the
;; logial AND, OR and XOR operations on them
;;

.ORIG 	x3000		; Initial addy
	LEA R6, STACK	; Initialze Stack start - any further, can't get
	ADD R6, R6, #-1	; Ready for first value, universal interface
	AND R4, R4, #0	; Clear R4
	AND R5, R5, #0	; Clear R5
	LEA R0, IPROMPT	; Output input prompt
	TRAP x22	; PUTS
	JSR INPUT	; Get the first number
	ADD R4, R4, R0	; Store number in R4
	LEA R0, IPROMPT	; Output input prompt
	TRAP x22	; PUTS
	JSR INPUT	; Get second number
	ADD R5, R5, R0	; Store number in R5

	ADD R6, R6, #1	; push R4
	STR R4, R6, #0	; pushed
	ADD R6, R6, #1	; push R5
	STR R5, R6, #0	; pushed
	JSR ANDL	; Preform logical AND
	AND R1, R1, #0	; clear R1, to store R0
	ADD R1, R1, R0	; R0->R1
	LEA R0, ANDBAN	; Output banner for and
	TRAP x22	; PUTS
	AND R0, R0, #0	; clear R0
	ADD R0, R0, R1	; R1->R0
	JSR OUTPUT	; Result in 0, output it

	ADD R6, R6, #1	; push R4
	STR R4, R6, #0	; pushed
	ADD R6, R6, #1	; push R5
	STR R5, R6, #0	; pushed
	JSR OR		; Logical OR
	AND R1, R1, #0	; clear R1, to store R0
	ADD R1, R1, R0	; R0->R1
	LEA R0, ORBAN	; Output banner for and
	TRAP x22	; PUTS
	AND R0, R0, #0	; clear R0
	ADD R0, R0, R1	; R1->R0
	JSR OUTPUT	; Again

	ADD R6, R6, #1	; push R4
	STR R4, R6, #0	; pushed
	ADD R6, R6, #1	; push R5
	STR R5, R6, #0	; pushed
	JSR XOR		; Logical XOR
	AND R1, R1, #0	; clear R1, to store R0
	ADD R1, R1, R0	; R0->R1
	LEA R0, XORBAN	; Output banner for and
	TRAP x22	; PUTS
	AND R0, R0, #0	; clear R0
	ADD R0, R0, R1	; R1->R0
	JSR OUTPUT	; Output it

STOP	HALT		; Done

STACK	.BLKW 100	; The stack
ANDBAN	.STRINGZ "\nAND: "
ORBAN	.STRINGZ "\nOR: "
XORBAN	.STRINGZ "\nXOR: "
IPROMPT	.STRINGZ "\nInput number: "


INPUT	ADD R6, R6, #1	; push R1
	STR R1, R6, #0	; pushed
	ADD R6, R6, #1	; push R2
	STR R2, R6, #0	; pushed
	ADD R6, R6, #1	; push R3
	STR R3, R6, #0	; pushed
	ADD R6, R6, #1	; push R4
	STR R4, R6, #0	; pushed
	ADD R6, R6, #1	; push R5
	STR R5, R6, #0	; pushed
	ADD R6, R6, #1	; push R7
	STR R7, R6, #0	; pushed
	AND R1, R1, #0	; Clear R1 - counter
	AND R3, R3, #0	; Clear R3 - return val
	ADD R1, R1, #-9	; Setup loop counter - CHECK NUMBER
	LD R4, ZERO	; Setup for checks
	NOT R4, R4	; Start negate
	ADD R4, R4, #1	; Negate R4
	LD R5, ONE	; Setup for checks
	NOT R5, R5	; Start negate
	ADD R5, R5, #1	; Negate R5
	LD R0, IPROMPT	; Prompt user for input
	TRAP x22	; PUTS prompt

ILOOP	ADD R1, R1, #1	; Add 1, have 8 digits?
	BRzp IDONE	; Done w/loop
	TRAP x20	; GETC->R0
	AND R2, R2, #0	; Clear R2 again
	ADD R2, R4, R0	; Is 0 if is '0' ;P
	BRz IZERO	; Goto handling code
	AND R2, R2, #0	; clear it again
	ADD R2, R5, R0	; Is 0 if is '1'
	BRz IONE	; Goto handling code
	ADD R1, R1, #-1	; Is not a good char
	BRnzp ILOOP	; Ignore this case, continue

IZERO	TRAP x21	; R0->OUT Let them see their typing, is good char
	ADD R3, R3, R3	; Left Shift
	BRnzp ILOOP	; Done here, continue input

IONE	TRAP x21	; R0->OUT Let them see their typing, is good char
	ADD R3, R3, R3	; Left Shift
	ADD R3, R3, #1	; Least sig fig = 1
	BRnzp ILOOP	; Go back to input

IDONE	AND R0, R0, #0	; Clear R0
	ADD R0, R0, R3	; Store Return in R0
	LDR R7, R6, #0	; pop R7
	ADD R6, R6, #-1	; popped
	LDR R5, R6, #0	; pop R5
	ADD R6, R6, #-1	; popped
	LDR R4, R6, #0	; pop R4
	ADD R6, R6, #-1	; popped
	LDR R3, R6, #0	; pop R3
	ADD R6, R6, #-1	; popped
	LDR R2, R6, #0	; pop R2
	ADD R6, R6, #-1	; popped
	LDR R1, R6, #0	; pop R1
	ADD R6, R6, #-1	; popped
	RET		; Done here, back to main



OUTPUT	ADD R6, R6, #1	; push R1
	STR R1, R6, #0	; pushed
	ADD R6, R6, #1	; push R2
	STR R2, R6, #0	; pushed
	ADD R6, R6, #1	; push R3
	STR R3, R6, #0	; pushed
	ADD R6, R6, #1	; push R4
	STR R4, R6, #0	; pushed
	ADD R6, R6, #1	; push R5
	STR R5, R6, #0	; pushed
	ADD R6, R6, #1	; push R7
	STR R7, R6, #0	; pushed
	AND R1, R1, #0	; Clear R1 - counter
	ADD R1, R1, #-9	; Setup loop counter - CHECK NUMBER
	LD R3, ZERO	; Setup for output
	LD R4, ONE	; Setup for output
	LD R5, OMASK	; Interesting idea I had... 
	

OLOOP1	ADD R1, R1, #1	; Loop down
	BRzp OUT2	; Done w/looping
	AND R2, R0, R5	; Bitmask R0 (input)
	BRz OZERO	; Code for 0
	BRp OONE	; Code for 1
	BRn STOP	; Had BETTER never happen
	
OZERO	ADD R6, R6, #1	; push '0'
	STR R3, R6, #0	; pushed
	ADD R5, R5, R5	; left shift R5, the bitmask...
	BRnzp OLOOP1	; back to loop

OONE	ADD R6, R6, #1	; push '1'
	STR R4, R6, #0	; pushed
	ADD R5, R5, R5	; left shift R5, the bitmask...
	BRnzp OLOOP1

OUT2	AND R1, R1, #0	; Clear R1 (just in case)
	ADD R1, R1, #-9	; setup for loop - CHECK NUMBER
OLOOP2	ADD R1, R1, #1	; Loop down
	BRzp ODONE	; done w/loop
	LDR R0, R6, #0	; pop R0
	ADD R6, R6, #-1	; popped
	TRAP x21	; Show them the result
	BRnzp OLOOP2	; next char

ODONE	LDR R7, R6, #0	; pop R7
	ADD R6, R6, #-1	; popped
	LDR R5, R6, #0	; pop R5
	ADD R6, R6, #-1	; popped
	LDR R4, R6, #0	; pop R4
	ADD R6, R6, #-1	; popped
	LDR R3, R6, #0	; pop R3
	ADD R6, R6, #-1	; popped
	LDR R2, R6, #0	; pop R2
	ADD R6, R6, #-1	; popped
	LDR R1, R6, #0	; pop R1
	ADD R6, R6, #-1	; popped
	RET		; Done here, back to main



ANDL	ADD R6, R6, #1	; push R1
	STR R1, R6, #0	; pushed
	ADD R6, R6, #1	; push R2
	STR R2, R6, #0	; pushed
	LDR R1, R6, #-2	; get arg 1 - feels wrong this way
	LDR R2, R6, #-3	; get arg 2

	AND R0, R1, R2	; Main code :P

	LDR R2, R6, #0	; pop R2
	ADD R6, R6, #-1	; popped
	LDR R1, R6, #0	; pop R1
	ADD R6, R6, #-1	; popped
	ADD R6, R6, #-2	; pop args
	RET		; Done here, back to main



OR	ADD R6, R6, #1	; push R1
	STR R1, R6, #0	; pushed
	ADD R6, R6, #1	; push R2
	STR R2, R6, #0	; pushed
	LDR R1, R6, #-2	; get arg 1 - feels wrong this way
	LDR R2, R6, #-3	; get arg 2

	NOT R1, R1	; main code, using DeMorgan's Law
	NOT R2, R2	; for OR = NOT ((NOT a) AND (NOT b))
	AND R0, R1, R2	; almost done
	NOT R0, R0	; very important; see above

	LDR R2, R6, #0	; pop R2
	ADD R6, R6, #-1	; popped
	LDR R1, R6, #0	; pop R1
	ADD R6, R6, #-1	; popped
	ADD R6, R6, #-2	; pop args
	RET		; Done here, back to main



XOR	ADD R6, R6, #1	; push R1
	STR R1, R6, #0	; pushed
	ADD R6, R6, #1	; push R2
	STR R2, R6, #0	; pushed
	ADD R6, R6, #1	; push R3
	STR R3, R6, #0	; pushed
	ADD R6, R6, #1	; push R4
	STR R4, R6, #0	; pushed
	ADD R6, R6, #1	; push R5
	STR R5, R6, #0	; pushed
	ADD R6, R6, #1	; push R7
	STR R7, R6, #0	; pushed
	LDR R1, R6, #-6	; get arg 1 - feels wrong this way
	LDR R2, R6, #-7	; get arg 2
	NOT R3, R1
	NOT R4, R2

	AND R5, R1, R4	; (ab')+(a'b) = XOR
	ADD R6, R6, #1	; push R5
	STR R5, R6, #0	; pushed
	AND R5, R2, R3
	ADD R6, R6, #1	; push R5
	STR R5, R6, #0	; pushed
	JSR OR		; OR the two results

	LDR R7, R6, #0	; pop R7
	ADD R6, R6, #-1	; popped
	LDR R5, R6, #0	; pop R5
	ADD R6, R6, #-1	; popped
	LDR R4, R6, #0	; pop R4
	ADD R6, R6, #-1	; popped
	LDR R3, R6, #0	; pop R3
	ADD R6, R6, #-1	; popped
	LDR R2, R6, #0	; pop R2
	ADD R6, R6, #-1	; popped
	LDR R1, R6, #0	; pop R1
	ADD R6, R6, #-1	; popped
	ADD R6, R6, #-2	; pop args
	RET		; Done here, back to main	



ZERO	.FILL x0030	; ASCII '0'
ONE	.FILL x0031	; ASCII '1'
OMASK	.FILL x0001
.END

































