.global set_cr3
set_cr3:
	movq %rdi,%rax 
	movq %rax,%cr3            	
	retq		
