; main.s - Bubblesort algorithm demonstration.
.data
.global X
.equ X, 40
array:
                .word 10, 42, 8, 34, 22 , 2, 1, 87, 99, 53, 67, 10, 69
                .word 22, 80, 11, 75, 25, 0, 12

size:           .word X


.text

.extern print_hex
.extern println

.global START
START:
                push &array
                push size
                call bubblesort
                add r6, 4

                mov r1, 0
loop:           push r1[array]
                call print_hex
                add r6, 2
                call println
                add r1, 2
                cmp size, r1
                jgt loop
exit:           halt

bubblesort:                     ; assumes size is greater than 0
                push r1
                push r2
                push r3
                push r4

                mov r3, r6[10]
                add r3, r6[12]
                ; sub r3, 2

next_iter:      cmp r3, r6[10]
                jeq $return

                mov r1, r6[10]
                mov r2, r1
inner_iter:     add r2, 2
                cmp r2, r3
                jeq $inner_iter_end

                mov r4, r2[0]
                cmp r4, r1[0]
                jgt $skip_swap
                push r1
                push r2
                call $swap
                add r6, 4

skip_swap:      mov r1, r2
                jmp $inner_iter

inner_iter_end: sub r3, 2
                jmp $next_iter
return:
                pop r4
                pop r3
                pop r2
                pop r1
                ret

swap:
                push r1
                push r2
                push r3

                mov r1, r6[8]
                mov r2, r6[10]

                mov r0, r1[0]
                mov r3, r2[0]
                mov r1[0], r3
                mov r2[0], r0

                pop r3
                pop r2
                pop r1
                ret

.end
