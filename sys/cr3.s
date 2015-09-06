.global cr3

cr3:
	movq %rdi, %rax
	movq %rax, %cr3
	retq
