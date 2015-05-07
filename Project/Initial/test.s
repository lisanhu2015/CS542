	.text	
	.globl		main
main:
	li		$t0, 2
	li		$v0, 1
	move		$a0, $t0
	syscall	
	li		$t0, 1
	neg		$t1, $t0
	move		$t0, $t1
	li		$t3, 0
	li		$t4, 1
L1:
	sub		$t0, $t0, $t4
	slt		$t2, $t0, $t3
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
	li		$v0, 10
	syscall	
	.data	
	.align		4
_nl:	.asciiz		"\n"
_sp:	.asciiz		" "
