	.text
_main:
	li $t0, 5
	move $s0, $t0
	li $v0, 1
	add $a0, $s0, $zero
	syscall
	li $v0, 10
	syscall
