#include <sys/task.h>
#include <sys/memory.h>
#include <sys/utilities.h>
#include <sys/paging.h>
#include <sys/tarfs.h>
#include <sys/gdt.h>
#define VADDR 0xFFFFFFFF80000000

/* Maintaing a virtual address	for mapping. Since we mapped 1000 kpages, the	
virtual address after that has to be maintained for further mapping in the page table */

//extern uint64_t *vir_pml4e; 
extern void tss_flush();
uint64_t virtual_addr = (0xFFFFFFFF80200000) + (4096 * 1000);			//	kernmem + (page_size * no.pages mapped)
uint64_t pid = 0;
uint64_t get_vaddr()
{
	return virtual_addr;
}

void set_vaddr(uint64_t a)
{
	virtual_addr = a;
}

/* 	Maintain a free-list for all the tasks		*/

struct task *free_list = NULL;
struct vma *vma_list = NULL;

struct task *get_free_task()
{
	struct task *current = NULL;
	if(free_list == NULL)
		return current;
	else
	{
		while(free_list != NULL)
		{
			current = free_list;
			free_list = free_list->next;
		}
		return current;
	}
}

struct vma *get_free_vma()
{
	struct vma *current = NULL;
	if(vma_list == NULL)
		return current;
	else
	{
		while(vma_list != NULL)
		{
			current = vma_list;
			vma_list = vma_list->next;
		}
		return current;
	}
}

void add_task_free_list(struct task *current)
{	
	if(free_list == NULL)
	{
		free_list = current;
		free_list->next = NULL;
	}
	else
	{
		while(free_list != NULL)
			free_list = free_list->next;
		free_list = current;
		free_list->next = NULL;
	}	
}

void add_vma_list(struct vma *current)
{	
	if(vma_list == NULL)
	{
		vma_list = current;
		vma_list->next = NULL;
	}
	else
	{
		while(vma_list != NULL)
			vma_list = vma_list->next;
		vma_list = current;
		vma_list->next = NULL;
	}	
}
/* Function to assign a task filled with details in its structure */

struct task *make_task(int process_type,char *name)
{
	struct task *task_current = get_free_task();
	
	/* 	If there are no free task available then we have to set the mm structures	*/
	
	if(task_current == NULL)
	{	
		struct mm_struct *mm_current = (struct mm_struct *)(VADDR+allocate_page());
		allocate_page();
		allocate_page();
		mm_current->vma_list = NULL;
		mm_current->vma_count = 0;
		mm_current->hiwater_vm = 0;
		mm_current->total_vm = 0;
		mm_current->stack_vm = 0;	
		
		task_current = (struct task*)(VADDR + allocate_page());
		allocate_page();
		allocate_page();
		task_current->mm = mm_current;
		task_current->next = NULL;
		task_current->prev = NULL;
		memset((void*)task_current->kernel_stack, 0, 512);
	}
	task_current->pid = pid++;
	task_current->ppid = 0;
	task_current->task_type = process_type;
	task_current->task_state = READY;
	strcpy(task_current->name,name);
	return task_current;	
}

struct vma *make_vma(uint64_t start, uint64_t end, uint64_t flags, uint64_t type, uint64_t fd)
{
	struct vma *vma = NULL;
	if ((vma = get_free_vma()) == NULL) 
	{
		vma = (struct vma*) (VADDR + allocate_page());
		allocate_page();
		allocate_page();
	}
	vma->vm_start = start;
	vma->vm_end = end;
	vma->vm_flags = flags;
	vma->vm_type = type;
	vma->next = NULL;
	vma->vm_file_descp = fd;
	return vma;
}

/* Making an idle process for testing */

/*void idle_process()
{
	struct task *current = make_task(1);		// Make task
	current->task_state = IDLE;			// set type if task
	strcpy (current->name,"IDLE TASK");		// set name of tassk
	//schedule_my_task(current, ,(uint64_t)&current->kernel_stack[511]);				// Schedule the task
}	
*/
/* Scheduling the process */

void schedule_my_task(struct task *current, uint64_t start, uint64_t top_of_stack)
{
      set_tss_rsp0((uint64_t)&current->kernel_stack[511]);
	__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(current->mm->pml4));
	
	__asm volatile(
	"movq %0, %%rsp;\n\t"
	"popq %%r15\n\t"
	"popq %%r14\n\t"
	"popq %%r13\n\t"
	"popq %%r12\n\t"
	"popq %%r11\n\t"	
	"popq %%r10\n\t"	
	"popq %%r9\n\t"	
	"popq %%r8\n\t"
	
	"popq %%rbp\n\t"
	"popq %%rdi\n\t"
	"popq %%rsi\n\t"
	"popq %%rdx\n\t"
	"popq %%rcx\n\t"
	"popq %%rbx\n\t"
	"popq %%rax\n\t"
	:
	:"r"(&(current->kernel_stack[490]))
	:"memory"
	);
	
	__asm__ __volatile__ ("movq $0x2b, %rax\n\t");
	__asm volatile("ltr %ax\n\t");
	
	__asm volatile(
	"iretq"
	);
}
