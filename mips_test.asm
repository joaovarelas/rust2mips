	.text
_main:
	li $t0, 10
	lw $s0, 0($t0)
_L0:
	lw $t0, 4($s0)
	li $t1, 1
	bge $t0, $t1, _L1
	j _L2
_L1:
	lw $t3, 4($s0)
	li $t4, 1
	sub $t2, $t3, $t4
	lw $s0, 4($t2)
	j _L0
_L2:
