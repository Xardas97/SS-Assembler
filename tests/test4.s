.text

.global main
.extern readln
.extern writeln

main:
    halt
    xchgb r0h, r0l
    xchg r0, r1
    xchgw r1, r2[]
    xchgw r2, r3[5]
    xchg r3, r4[offset]
    xchgw *782, r4
    xchg r5, &offset
    xchg sp, offset
    xchg sp, r6[-597]
    int 3
    mov r0, &offset
    addb r0h, r1l
    sub r2[], &offset
    mul r5[offset], r3
    div r0, $offset
    cmpb sp[-5], r0h
    not r5[0]
    andb r0h, r1l
    orw r0, r1[offset]
    xorb r0h, r0h
test:
    testb r0l, -5
    shl *0x682, r3
    shrb $offset, r2h
    push sp[-892]
    pop r3
    jmp test
    jeq r0[]
    jne *73
    jgt r4[offset]
    call func
    iret
    movw sp[0], 0x1234
    jmp $offset
func:
    push sp[4]
    ret

.data
.word 2
offset: .word 576

.section rodata 

hellostr:       .byte 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0x0

.end
