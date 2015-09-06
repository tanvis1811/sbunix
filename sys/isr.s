#
# irq_common.s -- IRQ common handler
#
.extern irq_handler
.extern page_fault_handler
.extern timer_handler
.extern keyboard_handler
.extern user_irq_handler
.global isr0
.global isr14
.global irq0
.global isr80
.global irq1
.global irq_common

.macro PUSHA

	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rsi
	pushq %rdi
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
.endm

.macro POPA
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax

.endm


irq_common:
	PUSHA
	movq %rsp, %rdi
	callq irq_handler
	POPA
	add $0x8, %rsp
	sti
	retq

irq0:
	cli
	PUSHA
		
	callq timer_handler
	POPA
	sti
	iretq

irq1:
	cli
	PUSHA
	movq %rsp, %rdi
	callq keyboard_handler
	POPA
	sti
	iretq

isr0:
	cli
	movq $0, %rdi
  	pushq %rdi 
	jmp irq_common
	#callq timer_handler
	retq
isr14:
	cli
	pushq $14
	pushq $14
	PUSHA
	movq %rsp, %rdi
	callq  page_fault_handler
	POPA
	add $0x10, %rsp
	sti
	iretq
	
isr80:
	cli
	PUSHA
	movq %rsp, %rdi
	callq user_irq_handler
	POPA
	sti
	iretq
