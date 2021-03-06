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
  		nop
  	
 	hexletters:			# if above 9 but below or equal to 15
 		addi $v0,$t0,0x37	# $t0 | 0x00 00 00 37 -> add 0x37 to what's input -> we get "ascii"
 		jr $ra			# letters accepted are A through F (10 through 15 decimal)
 		nop

	# if input is 17 we will throw away the 2nd nibble of 17:
 	# ..00 0001 0001 & ..00 0000 1111 leaving only the lower 0001 intact
 	# this is lower than or equal to 9, so addi 0001, 0x30 outputs 0x31
 	# which is ASCII for 1.
 	# in short: overflow is bad, since we discard all bits else than
 	# the first (lowest) nibble 

# placeholder delay function:
#delay:
#	jr $ra
#	nop

# non-placeholder delay
#
delay:
# less bad delay
	PUSH ($ra)

	li $t6, 180000	# constant
	move $t5, $a0	# called with argument $a0, move that to $t5 so we dare do stuff with it
	li $t4, 0	# i

	while_loop:
		ble $t5, 0, exit_delay	# if called with X < 0 go to exit
		nop
		sub $t5, $t5, 1	#ms -= 1	# for each iteration remove 1 from argument

	for_loop:
		bge $t4, $t6, while_loop	# if i <= argument break to while_loop
		nop
		addi $t4, $t4, 1	# i++
		j for_loop		# jump back to the top of the loop
		nop

	j while_loop
	nop

	exit_delay:
		POP ($ra)
		jr $ra
		nop

# possibly better delay?
#	PUSH ($ra)
#	PUSH ($a0)
	
#	li $v0, 32		# 32 is short for "sleep, time = $a0"
#	li $a0, 1000	# sleep for 1000ms
#	syscall			# invoke sleep

#	POP ($a0)
#	POP ($ra)

#	jr $ra
#	nop


	# NBCD handler
	#
	# $a0 contains the address where we can store our output
	# $a1 contains time info, lower 16 bits are of interest (0x0000 ffff)
time2string:
	PUSH ($s1)				# save $s1 in case someone else used it before
	PUSH ($ra)				# save return adress on stack

	# copy $a0 to $s1 so we can work with it without trouble
	move	$s1, $a0
	PUSH ($a0)				# push $a0 so that we're not doing anything stupid with it

	# first number
	andi	$t1, $a1, 0xf000 	# extract top nibble, shift different amount for each 4
	srl 	$a0, $t1, 12		# shift bits to lowest nibble
	jal 	hexasc
	nop							# jump instruction so NOP after
	sb 		$v0, 0($s1)			# store resulting "ascii" value in first pos of $s1
								# aligned so it goes 0(), 1(), 2(), 3() etc	

	# second number
	andi 	$t1, $a1, 0x0f00 	# extract top nibble, shift different amount for each 4
	srl 	$a0, $t1, 8			# shift bits to lowest nibble
	jal 	hexasc
	nop
	sb 		$v0, 1($s1)			# store resulting "ascii" value in second pos of $s1

	# :
	li 		$t1, 0x3A 			# insert : into output memory
	sb 		$t1, 2($s1)

	# Two more numbers 0x00f0 and 0x000f

	# third number
	andi 	$t1, $a1, 0x00f0 	# extract top nibble, shift different amount for each 4
	srl 	$a0, $t1, 4			# shift bits to lowest nibble
	jal 	hexasc
	nop
	sb 		$v0, 3($s1)			# store resulting "ascii" value in fourth pos of $s1


  # fourth number
  andi  $t1, $a1, 0x000f  # extract top nibble, shift different amount for each 4

  # jump to print NINE if last bit is 9
  li $t7, 9
  beq   $t1, $t7, printnine
  nop

  move  $a0, $t1      # since we dont need to SRL, move $t1 into $s1
  jal   hexasc
  nop
  move  $t2, $v0      # move return value from hexasc into $t2
  sb    $t2, 4($s1)     # store resulting "ascii" value in fifth pos of $s1

  # NUL
  li    $t1, 0x00       # insert NUL into output memory thing
  sb    $t1, 5($s1)

  POP $a0         # pop out what we pushed so as to not blow anything up
  POP $ra
  POP $s1

  jr    $ra         # jump out since we are done
  nop             # because we are scared of the branch delay slot

printnine:              #prints xx:xNINE if last nibble == 9
  
  #N
  li $t2, 0x4e      # put ASCII N in $t2
  sb $t2, 4($s1)    # store resulting N in right place

  #I
  li $t2, 0x49      # put ASCII I in $t2
  sb $t2, 5($s1)    # store resulting N in right place

  #N
  li $t2, 0x4e      # put ASCII N in $t2
  sb $t2, 6($s1)    # store resulting N in right place

  #E
  li $t2, 0x45      # put ASCII N in $t2
  sb $t2, 7($s1)    # store resulting N in right place

  # NUL
  li    $t1, 0x00       # insert NUL into output memory thing
  sb    $t1, 8($s1)

  POP $a0         # pop out what we pushed so as to not blow anything up
  POP $ra
  POP $s1

  jr    $ra         # jump out since we are done
  nop             # because we are scared of the branch delay slot
