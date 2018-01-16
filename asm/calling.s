	.file	"calling.c"
	.text
	.p2align 4,,15
	.globl	wat
	.type	wat, @function
wat:
.LFB0:
	.cfi_startproc
	movl	$3, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	wat, .-wat
	.ident	"GCC: (Debian 6.3.0-18) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
