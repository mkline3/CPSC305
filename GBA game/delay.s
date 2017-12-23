@ uppercase.s
/* This will kill time for when the program needs to pause a little */
.global delay
.global minus
.global add
delay:
	mov r3, #10
	mul r0, r0, r3
	mov r1, #1
	mov r2, #1
.top:
	add r1, r1, r2
	cmp r1, r0
	bne .top
	


minus:
	mov r1, #1
	sub r0, r0, r1
	mov pc, lr

add:
	mov r1, #1
	add r0, r0, r1
	mov pc, lr
