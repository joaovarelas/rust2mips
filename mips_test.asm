	.text
_main:
	li $t0, 0
	move $s0, $t0
	li $t0, 0
	move $s1, $t0
_L0:
	move $s2, $x
	li $t0, 100
	blt $t0, $t1, _L1
	j _L2
_L1:
_L3:
	move $s3, $y
	li $t0, 100
	blt $t2, $t3, _L4
	j _L5
_L4:
	move $s4, $y
	li $t0, 1
	add $t4, $t5, t6
	move $s5, $t4
	j _L3
_L5:
	move $s6, $x
	li $t0, 1
	add $t0, $t1, t2
	move $s7, $t0
	j _L0
_L2:
	li $v0, 1
	add $a0, x, $zero
	syscall
	li $v0, 1
	add $a0, y, $zero
	syscall
	li $v0, 10
	syscall
