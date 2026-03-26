;This file contains errorneous instractions
	mov #3
Func1: cmp 
	add MW, k:k
	sub r5 ,	#-5
	lea MATRIX[r0][	r4] KABEK
	clr r3 , r4
Hey:    not #2
	inc KABEK, #67,
	dec r1,r3
	jmp Hey
	bne MW, k:k
	jsr func1, #3
	red 45
	prn ,r4
	rts #3, @4
	stop
str:	.data 1,-10, W
	.matMAT[5][1]
	.string l"df"
