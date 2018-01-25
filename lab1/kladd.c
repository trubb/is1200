
time2string (address $a0, word $a1)

	output to $a0: content of $a1 decoded nibble by nibble for the lowest 4 nibbles
		(0x00 00 FF FF) -> SRL på en mask 0x00 00 F0 00 -> 0x0F00 -> 0x00F0 -> 0x000F

	do:






		PUSH ($ra)
		PUSH (ALLT VI VILL "RÅKA ANVÄNDA")

		// $t0 får hexasc använda, försök jobba med sparade egna register istället

		// copy $a0 to $t0 so we can work with it without trouble
		move $t0, $a0
		// push ($a0) ????????

		// first number
		andi $t1, $a1, 0xf000 		// extract top nibble, shift different amount for each 4
		srl $a0, $t1, 12			// shift bits to lowest nibble
		
		jal hexasc
		nop							// jump instruction so NOP after

		move $t2, $v0				// move return value from hexasc into $t2
		sb $t2, 0($t0)				// store resulting "ascii" value in first pos of $a0
		//alternativt
		sb $v0, 0($t0)
									// aligned so it goes 0(), 1(), 2(), 3() etc

		// second number
		andi $t1, $a1, 0x0f00 		// extract top nibble, shift different amount for each 4
		srl $a0, $t1, 8				// shift bits to lowest nibble
		
		jal hexasc
		nop							// jump instruction so NOP after

		move $t2, $v0				// move return value from hexasc into $t2
		sb $t2, 1($t0)				// store resulting "ascii" value in second pos of $a0


		li $t1, 0x3A 				// insert : into output memory thing
		sb $t1, 2($t0)

		/* two more numbers 0x00f0 and 0x000f */

		// third number
		andi $t1, $a1, 0x00f0 		// extract top nibble, shift different amount for each 4
		srl $a0, $t1, 4				// shift bits to lowest nibble
		
		jal hexasc
		nop							// jump instruction so NOP after

		move $t2, $v0				// move return value from hexasc into $t2
		sb $t2, 3($t0)				// store resulting "ascii" value in fourth pos of $a0


		// fourth number
		andi $t1, $a1, 0x000f 		// extract top nibble, shift different amount for each 4
		move $a0, $t1				// since we dont need to SRL, move $t1 into $a0
		
		jal hexasc
		nop							// jump instruction so NOP after

		move $t2, $v0				// move return value from hexasc into $t2
		sb $t2, 4($t0)				// store resulting "ascii" value in fifth pos of $a0


		li $t1, 0x00 				// insert NUL into output memory thing
		sb $t1, 5($t0)


		POP (ALLT VI VILL "RÅKA ANVÄNDA")
		POP ($ra)					// pop out that return adress so that we dont explode

		jr $ra						// jump out since we are done
		nop 						// because we are scared of doing things









		li $t1, 0xf000 		// create mask
		andi $t0 $a0, $t1 	
		srl 4(mask)			// mask shift right logical 4 steg
		PUSH ($a0)
		move $a0, $t0
		jal hexasc
		nop
		POP ($a0)

		sb $a0, 0/8/16/24/32/40($a1)

		j time2string


		0x0B 00 -> ASCII
		0X3A -> terminal output ( : )
		0x00 C0 -> ASCII
		0x00 0D -> ASCII
		0x00 -> terminal output (NUL)




void delay( int ms ) {
	
	int i;
	
	while ( ms > 0 ) {
		ms = ms - 1;
		for (int i = 0; i < 4711; i += 1)
		{
			/*sleep*/
		}
	}
}

delay:
	PUSH ($ra)
	PUSH ($a0)

	li $t6, 0		// int i = 0
	li $t5, 3000	// loop constant

		while_loop:
			ble $a0, 0, exit_delay	// if A then done, exit
			nop			// no operation
			sub $a0, $a0, 1		// ms = ms - 1
		
			for_loop:
				beq $t6, $t5, while_loop	//  jump out of loop when i = loop constant value
				nop
				addi $t6, $t6, 1	// i += 1 
				j for_loop			// loop
				nop

	exit_delay:
		POP ($a0)
		POP ($ra)
		jr $ra
		nop


delay2:

	PUSH ($ra)
	PUSH ($a0)
	
	li $v0, 32		// 32 is short for "sleep, time = $a0"
	li $a0, 1000	// sleep for 1000ms
	syscall			// invoke sleep

	POP ($a0)
	POP ($ra)

	jr $ra
	nop


goodguydelay:
	PUSH ($ra)

	li $t6, 3	# constant
	move $t5, $a0	# called with argument
	li $t4, 0	# i

	while_loop:
		ble $t5, 0, exit_delay	# if called with 0 go to exit
		sub $t5, $t5, 1	#ms -= 1	# for each iteration remove 1 from argument
		# no nop because it doesnt really matter here since if we exit it is time to leave

	for_loop:
		beq, $t4, $t6, while_loop	# if i <= argument break to while_loop
		nop
		addi $t4, t4, 1	# i++
		j for_loop		# jump back to the top of the loop
		nop

	j while_loop
	nop

	exit_delay:
		POP ($ra)
		jr $ra
		nop
