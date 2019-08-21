.data
test: .skip 100, 0xff
.align 8, 03, 2
test2: .skip 0x43, 057
.skip 0b10110110
.byte   0b00100010, 		  074, 	 -135, 		0xf2
.word 		0b0101010101010101,  	0356,   	6421, 	0x3f2a
 .word 		56,  	031  ,   	65536	 , 	0x3f2a

.equ num, 45

.text
.align 2, 057, 	 1 

.global MaIn
.extern printf, test, 		t2est

   MaIn:   
test3: mov	r0, &num	
	add r5, r2[0]		
	test r2[0x5], r0	
	and r4[536], r0		
	subb r2l, r3h		
	xchgw r0, sp		
	halt				

.section data2, rw
	n:  .word 0x195f
 TESTMatch:  .word 0x195f	   
   ;.L0: #
   _testLab_el12.3test: 
	023test:		   
	t$test:				
	test_:   

.section	  	rodata	 , 		a
.section 		 testsection2	 , 	rwx 

.end

junkdata: .word 0x54
