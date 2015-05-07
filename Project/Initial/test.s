	.text	
	.align		2
	.globl		main
main:
	li		$t0, 10
	li		$t1, 2
	add		$t2, $t0, $t1
	sw		$t2, num1
	li		$t0, 20
	li		$t1, 3
	mul		$t2, $t0, $t1
	lw		$t0, num1
	add		$t1, $t2, $t0
	sw		$t1, num2
	lw		$t0, num1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	lw		$t0, num2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	lw		$t0, num2
	li		$t1, 6
	li		$t2, 6
	li		$t3, 4
	neg		$t4, $t3
	neg		$t3, $t4
	div		$t2, $t3
	mfhi		$t4
	sub		$t2, $t1, $t4
	div		$t1, $t0, $t2
	sw		$t1, num2
	lw		$t0, num2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$t0, 1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	li		$t0, 2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$t0, 2
	move		$t1, $t0
	li		$t3, 0
	li		$t4, 1
L1:
	sub		$t1, $t1, $t4
	slt		$t2, $t1, $t3
	beq		$t2, $t3, L2
	beq		$t3, $t3, L3
L2:
	li		$v0, 4
	la		$a0, _sp
	syscall	
	beq		$t3, $t3, L1
L3:
	li		$t0, 3
	li		$v0, 1
	move		$a0, $t0
	syscall	
	.data	
L4:	.asciiz		"hello world!"
	.text	
	li		$v0, 4
	la		$a0, L4
	syscall	
	li		$v0, 10
	syscall	
	.data	
	.align		4
_nl:	.asciiz		"\n"
_sp:	.asciiz		" "
num1:	.word		0
num2:	.word		0
