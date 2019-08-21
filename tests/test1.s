.text
.global	find_x

.extern b

find_x:
	.word 0x3
	push K
.align 4
	jmp b
	mov r3, r2[12]		; r3 = n
	sub r3, 1			; r3--
	mov r1, r2[16]		; r1 = x
	mov r0, r2[8]		; r0 = &array

	cmp r3, 0		; if n <= 0 goto exit
	jgt b

a:	mov r2, r0[4*2]
	cmp r3, r2
	jeq l2
	jmp b

l2:	mov r0, r1
	pop r2	
	ret

.section newsection, rx
b:	.word 8
K:	.align 0x10 ,0xff , 0x10
.align 2,

.end
