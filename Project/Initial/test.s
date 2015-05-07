	.text
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
	li		$v0, 4
	la		$a0, _sp
	syscall	
	lw		$t0, num2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _sp
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
	li		$v0, 4
	la		$a0, _sp
	syscall	
	li		$t0, 1
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _sp
	syscall	
	li		$v0, 4
	la		$a0, _nl
	syscall	
	li		$t0, 2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$v0, 4
	la		$a0, _sp
	syscall	
	li		$v0, 10
	syscall	
	.data	
	.align		4
_nl:	.asciiz		"\n"
_sp:	.asciiz		" "
num1:	.word		0
num2:	.word		0
