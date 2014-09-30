;;; name: Michael Clark
;;; userid: mc467
;;; assignment/filename: lab6.asm
;;; grader: Scott
;;; date: 10-23-2003
;;; (fill in the above information)
;;; (note that your filename MUST be lab6.asm)
	
.ORIG	x3000
		LEA	R6,STACK

MAIN	LEA R0,INPROMPT
		PUTS
		AND R2,R2,#0
		JSR INPUT
		JSR CONVERT
		HALT


;;; This subroutine will input a character from the user and will
;;; return the decimal equivilant to the caller function.
;;; Only the digits 0-9 and the '\n' (newline) character should
;;; be accepted.  All other unacceptable input should be ignored.
;;; This subroutine must echo (print) valid input to the console.
INPUT	 STR R7, R6, #0	 ; Push R7
		 ADD R6, R6, #1
		 STR R1, R6, #0	 ; Push R1
		 ADD R6, R6, #1
		 STR R2, R6, #0	 ; Push R2
		 ADD R6, R6, #1
		 STR R3, R6, #0	 ; Push R3
		 ADD R6, R6, #1
		 STR R4, R6, #0	 ; Push R4
		 ADD R6, R6, #1
		 STR R5, R6, #0	 ; Push R5
		 ADD R6, R6, #1
		 AND R5, R5, #0	 ; Clear R5
		 LD R2, ENDL   	 ; Setup for checks
		 LD R3, ZERO
		 LD R4, NINE
	 	 
INLOOP	 GETC	 	   	 ; Trap x20
		 AND R1, R1, #0	 ; clear R1
		 ADD R1, R0, R2	 ; Is newline?
		 BRz INDONE	 	 ; Yup.  We done.
		 AND R1, R1, #0	 ; clear R1
		 ADD R1, R0, R3	 ; Is '0'?
		 BRn INLOOP	 	 ; Is less.  Bad.
		 AND R1, R1, #0	 ; clear R1
		 ADD R1, R0, R4	 ; Is '9'?
		 BRp INLOOP	 	 ; Is more.  Bad
		 OUT 			 ; Trap x21, is good char
		 AND R1, R1, #0	 ; Clear R1
		 ADD R1, R0, #0	 ; put the char in R1
		 AND R0, R0, #0	 ; Clear R0
		 ADD R0, R5, #0	 ; Put R5 in R0
		 JSR MULTIPLY	 ; Shift by one (decimal) sig fig.
		 AND R5, R5, #0	 ; Clear R5
		 ADD R5, R0, #0	 ; Put back in R5
		 ADD R1, R1, R3	 ; Turn into real number (-'0' is into num)
		 ADD R5, R5, R1	 ; Add onto end
		 ;JSR CHKOVFLW	 ; don't check for overflow
		 BRnzp INLOOP	 ; Go again!  (till done)
		 
INDONE	 OUT			 ; Output the newline.
		 AND R0, R0, #0	 ; Clear R0
		 ADD R0, R5, #0	 ; Put output in R0
		 ADD R6, R6, #-1 ; Pop R5
		 LDR R5, R6, #0
		 ADD R6, R6, #-1 ; Pop R4
		 LDR R4, R6, #0
		 ADD R6, R6, #-1 ; Pop R3
		 LDR R3, R6, #0
		 ADD R6, R6, #-1 ; Pop R2
		 LDR R2, R6, #0
		 ADD R6, R6, #-1 ; Pop R1
		 LDR R1, R6, #0
		 ADD R6, R6, #-1 ; Pop R7
		 LDR R7, R6, #0
	 	 RET

;;; This subroutine will accept a decimal value from R0.  It will multiply
;;; that value by 10.  It will then return the result in R0 to the
;;; caller function.
MULTIPLY   STR R7, R6, #0	 ; Push R7
		   ADD R6, R6, #1
		   STR R1, R6, #0	 ; Push R1
		   ADD R6, R6, #1
		   STR R2, R6, #0	 ; Push R2
		   ADD R6, R6, #1
		   AND R2, R2, #0	 ; clear R2, to store Ans
		   ADD R2, R0, #0	 ; ready to mult!
		   AND R1, R1, #0	 ; Clear R1
		   ADD R1, R1, #9	 ; setup for loop
		   
MULTLOOP   ADD R1, R1, #-1	 ; Are we done?
		   BRn MULTDONE		 ; If negative, Yup!
		   ADD R0, R0, R2	 ; Left shift, is same as x+x
		   BRnzp MULTLOOP	 ; Is good, keep going   
		   
MULTDONE   ADD R6, R6, #-1 ; Pop R2
		   LDR R2, R6, #0
		   ADD R6, R6, #-1 ; Pop R1
		   LDR R1, R6, #0
		   ADD R6, R6, #-1 ; Pop R7
		   LDR R7, R6, #0
		   RET


;;; This subroutine will accept a decimal value from R0.  It will convert
;;; the decimal number to its hexadecimal equivilant.  It will output
;;; the hex number to the console.
CONVERT		STR R7, R6, #0	 ; Push R7
		   	ADD R6, R6, #1
			STR R1, R6, #0	 ; Push R1
			ADD R6, R6, #1
			STR R2, R6, #0	 ; Push R2
			ADD R6, R6, #1
			STR R2, R6, #0	 ; Push R3
			ADD R6, R6, #1
			STR R4, R6, #0	 ; Push R4
			ADD R6, R6, #1
			STR R5, R6, #0	 ; Push R5
			ADD R6, R6, #1
			AND R1, R1, #0	 ; clear R1
			ADD R1, R0, #0	 ; Put R0 in R1
			AND R2, R2, #0	 ; clear R2
			LEA R3, TABLE	 ; setup for output
			ADD R5, R5, #4	 ; setup loop 1
			LEA R0,HEXLBL
			PUTS
			
LOOP1		ADD R5, R5, #-1	 ; decrement loop counter
			BRn CONVDONE	 ; done w/it all
			AND R4, R4, #0	 ; clear R4
			ADD R4, R4, #4	 ; setup loop 2
			AND R2, R2, #0	 ; clear R2
			
LOOP2		ADD R4, R4, #-1	 ; Decrement loop counter			
			BRn ENDLOOP2	 ; Done w/inner loop
			AND R1, R1, R1	 ; Set flags
			BRzp L2SHIFT	 ; high bit is 0
			ADD R2, R2, R2	 ; left shift R2
			ADD R2, R2, #1	 ; high bit is 1
			ADD R1, R1, R1	 ; left shift
			ADD R1, R1, #1	 ; and add 1, so is wrap shift
			BRnzp L2CONT
L2SHIFT		ADD R2, R2, R2	 ; shift left, defaults to 0
			ADD R1, R1, R1	 ; shift left, proper wrap
L2CONT		BRnzp LOOP2		 ; continue

ENDLOOP2	AND R0, R0, #0	 ; clear R0
			ADD R0, R3, R2	 ; Add offset to TABLE
			LDR R0, R0, #0	 ; why not?  Load R0 with the char
			OUT				 ; TRAP x21, show user char
			BRnzp LOOP1		 ; done, try for next char
			
CONVDONE	ADD R6, R6, #-1 ; Pop R5
			LDR R5, R6, #0
		 	ADD R6, R6, #-1 ; Pop R4
		 	LDR R4, R6, #0
		 	ADD R6, R6, #-1 ; Pop R3
		 	LDR R3, R6, #0
		 	ADD R6, R6, #-1 ; Pop R2
		 	LDR R2, R6, #0
		 	ADD R6, R6, #-1 ; Pop R1
		 	LDR R1, R6, #0
		 	ADD R6, R6, #-1 ; Pop R7
		 	LDR R7, R6, #0
			RET

;;; This subroutine is worth extra credit so it is up to you to determine
;;; how it should be written.
CHKOVFLW   STR R1, R6, #0	 ; Push R1
		   ADD R6, R6, #1
		   LD R1, PASTVAL	 ; get the old value
		   ST R0, PASTVAL	 ; put in the new one
		   
		   ; NO working code for it
		   
OVBAD	   LEA R0, OVFLWERR	 ; Error string
		   PUTS	   			 ; Let them know they baka desu.
		   HALT				 ; They baka, so don't care.  Done.
OVGOOD	   ADD R6, R6, #-1 ; Pop R1
		   LDR R1, R6, #0
		   RET

;;; pseudo-ops
;;; you must use these prompts in your code
INPROMPT	.STRINGZ "Enter decimal value:	"
OVFLWERR	.STRINGZ "\nERROR:  Overflow of numeric representation\n"
HEXLBL		.STRINGZ "0x"
ZERO		.FILL 	xFFD0		; -ASCII '0'
NINE		.FILL	xFFC7		; -ASCII '1'
ENDL		.FILL	xFFF6		; -Newline char ('\n')
TABLE		.STRINGZ "0123456789ABCDEF"
PASTVAL		.FILL	x0000		; used for overflow checking
STACK		.FILL	xffff
			.BLKW	40
.END
