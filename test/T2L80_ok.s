 #    2
 #    3
 #    4
 #    5
 #    6
 #    7
 #    8
				# b_global_decl (I, alignment = 4, size = 4)
.globl I
	.data
	.align	4
	.type	I, @object
	.size	I, 4
I:
	.zero	4
				# b_global_decl (J, alignment = 4, size = 4)
.globl J
	.align	4
	.type	J, @object
	.size	J, 4
J:
	.zero	4
				# b_global_decl (K, alignment = 4, size = 4)
.globl K
	.align	4
	.type	K, @object
	.size	K, 4
K:
	.zero	4
 #    9
				# b_global_decl (X, alignment = 4, size = 4)
.globl X
	.align	4
	.type	X, @object
	.size	X, 4
X:
	.zero	4
				# b_global_decl (Y, alignment = 4, size = 4)
.globl Y
	.align	4
	.type	Y, @object
	.size	Y, 4
Y:
	.zero	4
				# b_global_decl (Z, alignment = 4, size = 4)
.globl Z
	.align	4
	.type	Z, @object
	.size	Z, 4
Z:
	.zero	4
 #   10
				# b_global_decl (C, alignment = 1, size = 1)
.globl C
	.align	1
	.type	C, @object
	.size	C, 1
C:
	.zero	1
 #   11
				# b_global_decl (D, alignment = 8, size = 8)
.globl D
	.align	8
	.type	D, @object
	.size	D, 8
D:
	.zero	8
 #   12
				# b_global_decl (B, alignment = 1, size = 1)
.globl B
	.align	1
	.type	B, @object
	.size	B, 1
B:
	.zero	1
 #   13
 #   14
 #   15
 #   16
 #   17
 #   18
 #   19
 #   20
				# b_func_prologue (main)
	.text
.global main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
 #   21
 #   22
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_push_const_int (5)
	movl	$5, %eax
	subl	$8, %esp
	movl	%eax, (%esp)
				# b_assign (signed long int)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   23
				# b_push_ext_addr (J)
	subl	$8, %esp
	movl	$J, (%esp)
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_assign (signed long int)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   24
				# b_push_ext_addr (K)
	subl	$8, %esp
	movl	$K, (%esp)
				# b_push_const_int (14)
	movl	$14, %eax
	subl	$8, %esp
	movl	%eax, (%esp)
				# b_assign (signed long int)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   25
				# b_push_ext_addr (X)
	subl	$8, %esp
	movl	$X, (%esp)
				# b_push_const_double (3.1400000000000001e+00)
	.section	.rodata
	.align	8
.LC0:
	.long	1374389535
	.long	1074339512
	.text
	fldl	.LC0
	subl	$8, %esp
	fstpl	(%esp)
				# b_assign (float)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   26
				# b_push_ext_addr (Y)
	subl	$8, %esp
	movl	$Y, (%esp)
				# b_push_const_double (2.7100000000000000e+00)
	.section	.rodata
	.align	8
.LC1:
	.long	2061584302
	.long	1074114068
	.text
	fldl	.LC1
	subl	$8, %esp
	fstpl	(%esp)
				# b_assign (float)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   27
				# b_push_ext_addr (Z)
	subl	$8, %esp
	movl	$Z, (%esp)
				# b_push_const_double (0.0000000000000000e+00)
	.section	.rodata
	.align	8
.LC2:
	.long	0
	.long	0
	.text
	fldl	.LC2
	subl	$8, %esp
	fstpl	(%esp)
				# b_assign (float)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   28
				# b_push_ext_addr (D)
	subl	$8, %esp
	movl	$D, (%esp)
				# b_push_const_double (1.9199900000000000e+01)
	.section	.rodata
	.align	8
.LC3:
	.long	-1518700436
	.long	1077097260
	.text
	fldl	.LC3
	subl	$8, %esp
	fstpl	(%esp)
				# b_assign (double)
	fldl	(%esp)
	addl	$8, %esp
	movl	(%esp), %eax
	fstpl	(%eax)
	fstpl	(%esp)
				# b_pop ()
	addl	$8, %esp
 #   29
				# b_push_ext_addr (B)
	subl	$8, %esp
	movl	$B, (%esp)
				# b_push_const_int (0)
	movl	$0, %eax
	subl	$8, %esp
	movl	%eax, (%esp)
				# b_assign (signed char)
	movzbl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movb	%dl, (%eax)
	movb	%dl, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   30
				# b_push_ext_addr (C)
	subl	$8, %esp
	movl	$C, (%esp)
				# b_push_const_int (65)
	movl	$65, %eax
	subl	$8, %esp
	movl	%eax, (%esp)
				# b_convert (signed long int -> unsigned char)
				# b_assign (unsigned char)
	movzbl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movb	%dl, (%eax)
	movb	%dl, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   31
 #   32
				# b_alloc_arglist (0 bytes)
	movl	%esp, %eax
	subl	$4, %esp
	andl	$-16, %esp
	movl	%eax, (%esp)
	subl	$0, %esp
				# b_funcall_by_name (Print_globals, void)
	call	Print_globals
	addl	$0, %esp
	movl	(%esp), %ecx
	movl	%ecx, %esp
 #   33
 #   34
				# b_push_ext_addr (K)
	subl	$8, %esp
	movl	$K, (%esp)
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_push_ext_addr (J)
	subl	$8, %esp
	movl	$J, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_arith_rel_op ( + , signed long int)
	movl	(%esp), %ecx
	addl	$8, %esp
	movl	(%esp), %eax
	addl	%ecx, %eax
	movl	%eax, (%esp)
				# b_assign (signed long int)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   35
				# b_push_ext_addr (J)
	subl	$8, %esp
	movl	$J, (%esp)
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_push_ext_addr (K)
	subl	$8, %esp
	movl	$K, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_push_ext_addr (K)
	subl	$8, %esp
	movl	$K, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_arith_rel_op ( * , signed long int)
	movl	(%esp), %ecx
	addl	$8, %esp
	movl	(%esp), %eax
	imull	%ecx, %eax
	movl	%eax, (%esp)
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_push_ext_addr (K)
	subl	$8, %esp
	movl	$K, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_arith_rel_op ( + , signed long int)
	movl	(%esp), %ecx
	addl	$8, %esp
	movl	(%esp), %eax
	addl	%ecx, %eax
	movl	%eax, (%esp)
				# b_arith_rel_op ( / , signed long int)
	movl	(%esp), %ecx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%ecx
	movl	%eax, (%esp)
				# b_arith_rel_op ( - , signed long int)
	movl	(%esp), %ecx
	addl	$8, %esp
	movl	(%esp), %eax
	subl	%ecx, %eax
	movl	%eax, (%esp)
				# b_assign (signed long int)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   36
				# b_push_ext_addr (I)
	subl	$8, %esp
	movl	$I, (%esp)
				# b_push_ext_addr (K)
	subl	$8, %esp
	movl	$K, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_push_const_int (7)
	movl	$7, %eax
	subl	$8, %esp
	movl	%eax, (%esp)
				# b_arith_rel_op ( % , signed long int)
	movl	(%esp), %ecx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%ecx
	movl	%edx, (%esp)
				# b_assign (signed long int)
	movl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movl	%edx, (%eax)
	movl	%edx, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   37
				# b_push_ext_addr (B)
	subl	$8, %esp
	movl	$B, (%esp)
				# b_push_ext_addr (K)
	subl	$8, %esp
	movl	$K, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_push_ext_addr (J)
	subl	$8, %esp
	movl	$J, (%esp)
				# b_deref (signed long int)
	movl	(%esp), %eax
	movl	(%eax), %edx
	movl	%edx, (%esp)
				# b_arith_rel_op ( > , signed long int)
	movl	(%esp), %ecx
	addl	$8, %esp
	movl	(%esp), %eax
	cmpl	%ecx, %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, (%esp)
				# b_convert (signed long int -> signed char)
				# b_assign (signed char)
	movzbl	(%esp), %edx
	addl	$8, %esp
	movl	(%esp), %eax
	movb	%dl, (%eax)
	movb	%dl, (%esp)
				# b_pop ()
	addl	$8, %esp
 #   38
				# b_push_ext_addr (C)
	subl	$8, %esp
	movl	$C, (%esp)
				# b_push_ext_addr 