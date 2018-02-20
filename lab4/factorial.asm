# n = $a0
# return = $v0
# product = $a1
# i = $a2
# j = $a3
# k = $v1


addi $a0, $0, 8		# $a0 = n = 0,3,8
addi $v0, $0, 1		# return value = 1 to start with (!0 == 1)
addi $v1, $a0, 1	# k = n + 1
addi $a2, $0, 1		# initialise outer loop var i to 1

loop:
	beq $a2, $v1, done	# i != k, if i(a2) == k(v1) we're done
	addi $a2, $a2, 1	# i++, increment loop variable
	addi $a1, $0, 0		# initialise the product to 0
	
	addi $a3,$0, 1		# initialise inner loop var j to 1
	
	multi:
		beq $a3, $a2, out	# if j == i, if a3 == a2 done "multiadding"
		addi $a3, $a3, 1	# j++, increment inner loop variable
		add $a1, $a1, $v0	# product == product + return
		beq $0, $0, multi	# jump back to mul
		
	out:
		addi $v0, $a1, 0		# set return variable to product of "multiadding"
		beq $0, $0, loop		# jump back to outer loop
	
done:
	beq $0, $0, done		# infinite unconditional branch loop
