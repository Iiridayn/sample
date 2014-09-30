	.ORIG	x0200
;;; Test ADD instructions (assume registers reset to zero on
;;; initialization)
	ADD	R1,R1,#5	; R1:	0x5
	ADD	R2,R2,#-2	; R2:	0xfffe
	ADD	R3,R2,R1	; R3:	0x3
	HALT
	.END