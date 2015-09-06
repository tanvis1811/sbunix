#include <sys/task.h>
#include <sys/memory.h>
#include <sys/utilities.h>
#include <sys/paging.h>
#include <sys/sbunix.h>
#include <sys/timer.h>
#include <sys/tarfs.h>
#include <sys/gdt.h>
#include <sys/process.h>
#include <sys/sbunix.h>

#define VADDR 0xFFFFFFFF80000000
extern char kernmem, physbase;
/*
	63 = ds = 0x23
	62 = rip = ?
	61 = eflags = 0x246 / 0x200286
	60 = cs = 0x1b
	59 = address of foo/bar
*/

struct task *task1 = NULL;
struct task *task2 = NULL;

struct task *head_list = NULL;
static int c = 0;

void add_task_tail_list (struct task *proc)
{
	struct task *list = head_list;
	if(head_list == NULL)
	{
		head_list = proc;
		head_list->next = proc;
	}
	else
	{
		while(list->next != (head_list))
			list = list->next;
		list->next = proc;
		list = list->next;
		list->next = head_list;
	}
}

void yeild(struct task **head) 
{      
	struct task *current = (*head);
	running = current->next;

	current = current->next;
	
	/* Now change the %rsp to callee rsp */
	__asm volatile(
		"movq %0, %%rsp;"
		:
		:"r"((current->rsp))
	        :"memory"
	);
	__asm volatile(
		"popq %r15"
		);
	__asm volatile(
		"popq %r14"
		);
	__asm volatile(
		"popq %r13"
		);
		
	__asm volatile(
		"popq %r12"
		);
	__asm volatile(
		"popq %r11"
		);

	__asm volatile(
		"popq %r10"
		);
	__asm volatile(
		"popq %r9"
		);
	__asm volatile(
		"popq %r8"
		);
	__asm volatile(
		"popq %rbp"
		);
	__asm volatile(
		"popq %rdi"
		);
	__asm volatile(
		"popq %rsi"
		);
	__asm volatile(
		"popq %rdx"
		);
	
	__asm volatile(
		"popq %rcx"
		);
	__asm volatile(
		"popq %rbx"
		);
	__asm volatile(
		"popq %rax"
		);
	
	__asm volatile(
		"iretq;"
	);
}

void thread1()
{	
	while(1)
	printf("hello\n");
}

void thread2()
{
	while(1)
  	printf("world\n");
}

void initialize_stack1()
{
	uint64_t physaddr = allocate_page();
	allocate_page();
	allocate_page();
	task1 = (struct task*)(VADDR + physaddr);
	strcpy(task1->name,"thread1");
	
	task1->rsp = (uint64_t) &(task1->kernel_stack[511]);
	task1->kernel_stack[511] = (uint64_t) &thread1;
	add_task_tail_list(task1);
	
			//while(1);
}

void initialize_stack(struct task *current, uint64_t start, uint64_t top_of_stack,int add)
{
	++c;
	current->kernel_stack[509] = 0x23;
	current->kernel_stack[506] = 0x1b;
	current->kernel_stack[508] = (uint64_t)top_of_stack;
	current->kernel_stack[507] = 0x246;
	set_tss_rsp0((uint64_t)&current->kernel_stack[511]);
	current->rsp = (uint64_t) &(current->kernel_stack[490]);
	current->kernel_stack[505] = start;
	current->kernel_stack[504] = 0;

	if(c == 1)	
	running = current; 
	if (add)
	add_task_tail_list(current);	
}

void scheduler()
{
	if(running != NULL && running->next != NULL)
	{	
		struct task* temp = running;		
		while(temp->next->task_state == WAITING)	
		{
			temp = temp->next;	
		}
		__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(temp->next->mm->pml4));
		set_tss_rsp0((uint64_t)&temp->next->kernel_stack[511]);	
		yeild(&temp);						
	}
}

void start_executing()
{
	running = head_list;
	__asm__ __volatile__ ("movq %0, %%rsp;" :: "m"(task1->rsp));
	__asm volatile("retq;");
	
}

int fork()
{
	uint64_t start, end, size;
	int nofpages;
	int i = 0;
	struct task *parent_proc = running;
	struct task *child_proc = make_task(1,parent_proc->name);
	uint64_t c_phy_pml4e = allocate_page();
	child_proc->mm->pml4 = c_phy_pml4e;
	uint64_t *child_vir_pml4e = (uint64_t *)(VADDR + c_phy_pml4e);
	uint64_t *v_pml4 = (uint64_t *)(VADDR + pml4);
	child_vir_pml4e[511] = v_pml4[511]; 

	// Copying the parent mm struct into child
	memcpy(child_proc->mm,parent_proc->mm,sizeof(struct mm_struct));
	child_proc->mm->pml4 = c_phy_pml4e; 
	child_proc->mm->vma_list = NULL;
	child_proc->ppid = parent_proc->pid;
	struct vma *my_vma = parent_proc->mm->vma_list;
			
	// Now copy all the parent vmas into child and also map the child to corresponding elf vaddr
	while(my_vma != NULL)
	{
		start = my_vma->vm_start;
		end = my_vma->vm_end;
		size = end - start;
		nofpages=(size/4096);
		if(size%4096>0)
			nofpages=nofpages+1;
		
		
		// Add the current vma to the vma list in mm struct of task 
		struct vma *current_vma = make_vma(start, end, my_vma->vm_flags, my_vma->vm_type, my_vma->vm_file_descp);
		
		/*  Copying the parent user stack into child user stack*/
		if(my_vma->vm_type == STACK)
		{

			uint64_t temp = (VADDR + allocate_page());
			for(i=1;i<=9;i++)
			{
				allocate_page();
			}
			uint64_t *temp1 = (uint64_t*)temp;
	
			// load parent pml4 and copy in temp page
			__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(parent_proc->mm->pml4));
	
		
			uint64_t *parent_stack = (uint64_t*)(end) - 5120;
			for(i =0;i<5119;i++)
			{
				temp1[i] = parent_stack[i];
			}

			// Load saved cr3
			
			__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(child_proc->mm->pml4)); 	
		
			proc_paging( end-(512*8*10),10, child_vir_pml4e);
			uint64_t *child_stack = (uint64_t*)(end) - 5120;
			for(i =0;i<5119;i++)
			{
				child_stack[i] = temp1[i];
			}
			__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pml4));

			add_vma_mm_list(current_vma, &child_proc);	
			
		}		
		else
		{
			// load child pml4
			__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(child_proc->mm->pml4));	
	
			// Mapping all the pages
			proc_paging(start,nofpages,child_vir_pml4e);

			memcpy((void*)start,(void*)my_vma->ph, (int64_t)(my_vma->mem_size));

			// Load saved cr3
			__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pml4));	
		}
			
		my_vma = my_vma->next; 
	}
	// Copy parent kernel stack into child kernel stack
	
	for (i = 0; i < 512; i++)
	{
		child_proc->kernel_stack[i] = parent_proc->kernel_stack[i]; 
	}
	//child_proc->kernel_stack[507] = parent_proc->kernel_stack[502];
	child_proc->kernel_stack[507] = 0x200286;
	child_proc->kernel_stack[504] = 0;	
	child_proc->rsp = (uint64_t) &(child_proc->kernel_stack[490]);	

	// Add the chidd to the end of the list
	add_task_tail_list(child_proc);
	
	/*  Loading back the parent pml4*/
	__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(parent_proc->mm->pml4));		
	return child_proc->pid;
}

int getpid()
{
	return running->pid;
}

int getppid()
{
	return running->ppid;
}

int execve(char *filename, char *argv[], char *envp[])
{
	struct task* child=running;
	struct task *new_proc=load_elf( filename,argv,1);
	if(new_proc == NULL)
		return -1;
	new_proc->pid=child->pid;
	new_proc->ppid=child->ppid;

	make_new_running_proc(new_proc);
	scheduler();
	set_tss_rsp0((uint64_t)&new_proc->kernel_stack[511]);	
	return 1;
	 
}


void make_new_running_proc( struct task* new_proc)
{   
	struct task* temp=running;
	while(temp->next!=running)
		temp=temp->next;
	temp->next=running->next;
	
}	

int exit()
{
	struct task *delete = running;
	pid_t pid = getppid();
	struct task *temp = running;
	while(temp->next->pid != pid)
		temp = temp->next;
	temp->next->task_state = READY;
	while(delete->next != running)
		delete = delete->next;
	delete->next = running->next;
	scheduler();
	return 1;	
}

int waitpid()
{
	struct task *waiting = running;
	waiting->task_state = WAITING;
	scheduler(); 
	return 1;
}

uint64_t ksbrk(uint64_t size)
{
	int npage=size/4096;
	
	if((size%4096)!=0)
		npage++;
	
	uint64_t *new_vir_pml4e=NULL;
	new_vir_pml4e = (uint64_t *)(running->mm->pml4+VADDR);
	
	uint64_t val=running->mm->start_brk;
	
	
	running->mm->start_brk=running->mm->start_brk+size;
	
	int dif=(int)(running->mm->top_heap)-(int)(running->mm->start_brk);
	
	if(dif<(int)size)
	{
		running->mm->top_heap=running->mm->top_heap+(npage*4096);  //.........increase brk
		proc_paging(val,npage,new_vir_pml4e);
	}
	return val;
}
