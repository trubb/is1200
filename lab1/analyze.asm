  # analyze.asm
  # This file written 2015 by F Lundevall
  # Copyright abandoned - this file is in the public domain.
  # changed 2018-01-23 by E. Pettersson and S. Andersson

	.text
main:
	li	$s0,0x30		# load 0x30 into $s0 (0x30 == ascii char '0')
loop:
	move	$a0,$s0		# copy from s0 to a0
	
	li	$v0,11			# syscall with v0 = 11 will print out
	syscall				# one byte from a0 to the Run I/O window

	addi	$s0,$s0,3	# what happens if the constant is changed?
						# if constant is changed we will jump in different increments, to print the requested string 1 -> 3
						# we print the next character when syscall ($v0 == 11) happens
	
	li	$t0,0x5b		# load 0x0000005b into $t0, 0x5b == ascii char '[', we dont want to print this so:
	bne	$s0,$t0,loop	# if $s0 != $t0, jump to loop (top of function), if $s0 contains 0x5b after all the addi's above we
						# get a FALSE condition and will continue to execute code below: the "stop" loop
	nop					# delay slot filler (just in case)

stop:	j	stop		# loop forever here
	nop					# delay slot filler (just in case)

