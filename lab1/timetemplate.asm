  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.
  # Modified 2018-01-24 by E. Pettersson and S. Andersson

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"

	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,2
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop

# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit

tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #
hexasc:
  	andi $t0,$a0,0xf		# $a0 & 0x0000000f -> ignore all bits but the bits in least significant end
  
 	ble $t0,9,hexnumbers 		# branch if content of $t0 <= 9, else continue
 	nop
 	ble $t0,31,hexletters		# branch if conten of $t0 <= 15, else continue
 	nop						# check if this is needed, might be better to jr $ra
  
  	hexnumbers:			# if below or equal to 9
  		addi $v0,$t0,0x30	# $t0 | 0x00 00 00 30 -> add 0x30 to what's input -> we get "ascii"
  		jr $ra			# numbers 0 through 9 (0 through 9 decimal)
  	
 	hexletters:			# if above 9 but below or equal to 15
 		addi $v0,$t0,0x37	# $t0 | 0x00 00 00 37 -> add 0x37 to what's input -> we get "ascii"
 		jr $ra			# letters accepted are A through F (10 through 15 decimal)

	# if input is 17 we will throw away the 2nd nibble of 17:
 	# ..00 0001 0001 & ..00 0000 1111 leaving only the lower 0001 intact
 	# this is lower than or equal to 9, so addi 0001, 0x30 outputs 0x31
 	# which is ASCII for 1.
 	# in short: overflow is bad, since we discard all bits else than
 	# the first (lowest) nibble 

# placeholder delay function:
delay:
	jr $ra
	nop

	# NBCD handler
	#
time2string:
	PUSH ($s1)
	PUSH ($ra) 				# save return adress on stack
#	PUSH (MORE STUFF)

	# copy $a0 to $s1 so we can work with it without trouble
	move $s1, $a0
#	PUSH ($a0)				# In case we need to push $a0

	# first number
	andi $t1, $a1, 0xf000 		# extract top nibble, shift different amount for each 4
	srl $a0, $t1, 12			# shift bits to lowest nibble
	
	jal hexasc
	nop							# jump instruction so NOP after

	sb $v0, 0($s1)				# store resulting "ascii" value in first pos of $s1
								# aligned so it goes 0(), 1(), 2(), 3() etc	

	# second number
	andi $t1, $a1, 0x0f00 		# extract top nibble, shift different amount for each 4
	srl $a0, $t1, 8				# shift bits to lowest nibble
	
	jal hexasc
	nop							# jump instruction so NOP after

	sb $v0, 1($s1)				# store resulting "ascii" value in second pos of $s1

	# :
	li $t1, 0x3A 				# insert : into output memory
	sb $t1, 2($s1)

	# Two more numbers 0x00f0 and 0x000f

	# third number
	andi $t1, $a1, 0x00f0 		# extract top nibble, shift different amount for each 4
	srl $a0, $t1, 4				# shift bits to lowest nibble
	
	jal hexasc
	nop							# jump instruction so NOP after

	sb $v0, 3($s1)				# store resulting "ascii" value in fourth pos of $s1

	# fourth number
	andi $t1, $a1, 0x000f 		# extract top nibble, shift different amount for each 4
	move $a0, $t1				# since we dont need to SRL, move $t1 into $s1
	
	jal hexasc
	nop							# jump instruction so NOP after

	move $t2, $v0				# move return value from hexasc into $t2
	sb $t2, 4($s1)				# store resulting "ascii" value in fifth pos of $s1

	# NUL
	li $t1, 0x00 				# insert NUL into output memory thing
	sb $t1, 5($s1)


#	POP ($a0)
	POP ($ra)					# pop out that return adress so that we dont explode
	POP ($s1)

	jr $ra						# jump out since we are done
	nop 						# because we are scared of doing things
