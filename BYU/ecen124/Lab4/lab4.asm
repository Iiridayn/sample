;; Name: Michael Clark
;; Grader: Scott
;; Assignment: lab4
;;
;; Checks an input string to see if it is a pallendrome
;;
.ORIG 	x3000		; Initial addy
	LD R5, SPACE	; Loads space for later tests
	LD R6, ENTER	; Loads enter char for later tests

INIT	LEA R4, PALLY	; (Re)sets up head pointer for Pallendrome
	LEA R3, PALLY	; (Re)sets up tail pointer for Pallendrome
	LEA R0, START	; Sets up for next instruction
	PUTS		; Prints a prompt of sorts

INLOOP	GETC		; Puts user input char in R0
	OUT		; Let them see what they type...
	ADD R1, R0, R5	; Setup check for space
	BRz INLOOP	; Ignore space
	ADD R1, R0, R6	; Setup check for '\n'
	BRz PALCK	; Goto Pallendrome checker code
	STR R0, R3, #0	; Put char into memory, at tail
	ADD R3, R3, #1	; Increment R3 (tail pointer)
	BRnzp INLOOP	; Not done reading input yet...

PALCK	NOT R2, R4	; Invert R4, setup for R3-R4
	ADD R2, R2, #1	; To negate, invert and add 1...
	ADD R1, R3, R2	; Setup check for pointers
	BRz NO		; If is empty, not pallendrome (though technically is...)
	ADD R4, R4, #-1	; Move head so can be used normally

PALOOP	ADD R4, R4, #1	; Move R4 up one
	ADD R3, R3, #-1	; Move R3 back one
	NOT R2, R4	; Invert R4, setup for R3-R4
	ADD R2, R2, #1	; To negate, invert and add 1...
	ADD R1, R3, R2	; Setup check for pointers
	BRnz YES	; Done with Pallendrome, success!
	LDR R0, R3, #0	; Get one of the chars
	LDR R1, R4, #0	; Get the other char (for test)
	NOT R0, R0	; Start negation of one char
	ADD R0, R0, #1	; Done negating first char
	ADD R2, R1, R0	; Setup for test (R1-R0)
	BRz PALOOP	; Are equal; keep checking
	
NO	LEA R0, FAIL	; Not equal, load fail
	PUTS		; Output fail string
	BRnzp TEST	; Check if want again

YES	LEA R0, ISPAL	; Is a pallendrome!
	PUTS		; Let them know too...

TEST	LEA R0, AGAIN	; Play again?
	PUTS		; Ask...
	LD R1, Y	; Setup for a test
	LD R2, N	; Setup for other test
	GETC		; Get user input
	OUT		; Let them see too...
	ADD R1, R1, R0	; Is it 'y'?
	BRz INIT	; Yes! Restart.
	ADD R2, R2, R0	; Is it 'n' then?
	BRz STOP	; yup.  done
	LEA R0, STUPID	; user no b4k4^2
	PUTS		; Let them know :P
	BRnzp TEST	; Try again please.

STOP	HALT		; Self explanatory
	
	

ENTER	.FILL	xFFF6	; negated ASCII '\n' char (x0A)
SPACE	.FILL	xFFE0	; negated ASCII ' ' char (x20)
Y	.FILL	xFF87	; negated ASCII 'y' char (x79)
N	.FILL	xFF92	; negated ASCII 'n' char (x6E)
START	.STRINGZ "\nPlease enter a potential pallendrome: "
AGAIN	.STRINGZ "\nWould you like to test another (y/n)? "
FAIL	.STRINGZ "\nThat was not a pallendrome."
ISPAL	.STRINGZ "\nThat was a pallendrome!"
STUPID	.STRINGZ "\nLowercase 'y' or 'n' please..."
PALLY	.BLKW	80	; Sets up for user input
	.END