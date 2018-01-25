  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.
  # Modified 2018-01-24 by E. Pettersson and S. Andersson

.global delay
.global time2string
  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #

  .text

hexasc:
    andi $t0,$a0,0xf    # $a0 & 0x0000000f -> ignore all bits but the bits in least significant end
  
  ble $t0,9,hexnumbers    # branch if content of $t0 <= 9, else continue
  nop
  ble $t0,31,hexletters   # branch if conten of $t0 <= 15, else continue
  nop           # check if this is needed, might be better to jr $ra
  
    hexnumbers:     # if below or equal to 9
      addi $v0,$t0,0x30 # $t0 | 0x00 00 00 30 -> add 0x30 to what's input -> we get "ascii"
      jr $ra      # numbers 0 through 9 (0 through 9 decimal)
      nop
    
  hexletters:     # if above 9 but below or equal to 15
    addi $v0,$t0,0x37 # $t0 | 0x00 00 00 37 -> add 0x37 to what's input -> we get "ascii"
    jr $ra      # letters accepted are A through F (10 through 15 decimal)
    nop

  # fi input is 17 we will throw away the 2nd nibble of 17:
  # ..00 0001 0001 & ..00 0000 1111 leaving only the lower 0001 intact
  # this is lower than or equal to 9, so addi 0001, 0x30 outputs 0x31
  # which is ASCII for 1.
  # in short: overflow is bad, since we discard all bits else than
  # the first (lowest) nibble 

# placeholder delay function:
#delay:
# jr $ra
# nop

# non-placeholder delay
#
delay:
# less bad delay
  PUSH $ra

  li $t6, 4711  # constant
  move $t5, $a0 # called with argument $a0, move that to $t5 so we dare do stuff with it
  li $t4, 0 # i

  while_loop:
    ble $t5, 0, exit_delay  # if called with X < 0 go to exit
    nop
    sub $t5, $t5, 1 #ms -= 1  # for each iteration remove 1 from argument

  for_loop:
    bge $t4, $t6, while_loop # if i >= argument break to while_loop
    nop
    addi $t4, $t4, 1  # i++
    j for_loop    # jump back to the top of the loop
    nop

  j while_loop
  nop

  exit_delay:
    POP $ra
    jr $ra
    nop

# possibly better delay?
# PUSH ($ra)
# PUSH ($a0)
  
# li $v0, 32    # 32 is short for "sleep, time = $a0"
# li $a0, 1000  # sleep for 1000ms
# syscall     # invoke sleep

# POP ($a0)
# POP ($ra)

# jr $ra
# nop


  # NBCD handler
  #
  # $a0 contains the address where we can store our output
  # $a1 contains time info, lower 16 bits are of interest (0x0000 ffff)
time2string:
  PUSH $s1        # save $s1 in case someone else used it before
  PUSH $ra        # save return adress on stack

  # copy $a0 to $s1 so we can work with it without trouble
  move  $s1, $a0
  PUSH $a0        # push $a0 so that we're not doing anything stupid with it

  # first number
  andi  $t1, $a1, 0xf000  # extract top nibble, shift different amount for each 4
  srl   $a0, $t1, 12    # shift bits to lowest nibble
  jal   hexasc
  nop             # jump instruction so NOP after
  sb    $v0, 0($s1)     # store resulting "ascii" value in first pos of $s1
                # aligned so it goes 0(), 1(), 2(), 3() etc 

  # second number
  andi  $t1, $a1, 0x0f00  # extract top nibble, shift different amount for each 4
  srl   $a0, $t1, 8     # shift bits to lowest nibble
  jal   hexasc
  nop
  sb    $v0, 1($s1)     # store resulting "ascii" value in second pos of $s1

  # :
  li    $t1, 0x3A       # insert : into output memory
  sb    $t1, 2($s1)

  # Two more numbers 0x00f0 and 0x000f

  # third number
  andi  $t1, $a1, 0x00f0  # extract top nibble, shift different amount for each 4
  srl   $a0, $t1, 4     # shift bits to lowest nibble
  jal   hexasc
  nop
  sb    $v0, 3($s1)     # store resulting "ascii" value in fourth pos of $s1

  # fourth number
  andi  $t1, $a1, 0x000f  # extract top nibble, shift different amount for each 4
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
