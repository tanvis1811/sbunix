#include<sys/task.h>
#include<sys/tarfs.h>
#include <sys/memory.h>
#include<sys/paging.h>
#include<sys/elf.h>
#include <sys/utilities.h>
#define VADDR 0xFFFFFFFF80000000

volatile int bbb=0;

int check_elf(struct elf64_ehdr* hdr)
{
	
	if((hdr->e_ident[1]=='E') &&(hdr->e_ident[2]=='L')&&(hdr->e_ident[3]=='F'))
	{
		return 1;
	}
	return 0;
}

void add_vma_mm_list(struct vma *current_vma, struct vma **vma_list)
{
	struct vma *temp = (*vma_list);
	if ((*vma_list) == NULL)
	{
		(*vma_list) = current_vma;
		(*vma_list)->next = NULL;
		return;
	}
	else
	{
		while(temp->next != NULL)
		{
			temp = temp->next;
		}	
		temp->next = current_vma;
		temp->next->next = NULL;
		return;
	}
} 

struct task *load_elf(char *filename,char *argv[],int add)
{
	struct elf64_ehdr* ehdr=(struct elf64_ehdr*) lookup(filename);
	
	if(ehdr==NULL)
	{
		printf("File not there!\n");
		return NULL;
	}
		
	
	Elf64_Phdr *ph;
	int c=check_elf(ehdr);
	if(c==0)
	{
		return 0;
	}
	struct task *proc = make_task(1,filename);	// for user process give arg as 1 otherwise 0
	
	ph = (Elf64_Phdr*) ((void*)ehdr + ehdr->e_phoff);
	uint64_t start_vaddr, nofpages, end_vaddr, max_addr = 0, vm_type;
	
	//uint64_t pml4;
	uint64_t *new_vir_pml4e=NULL;
	int i, size;
	// Save current PML4 table
	__asm__ __volatile__ ("movq %%cr3, %0;" : "=r"(pml4));

	uint64_t* v_pml4=NULL;
	 
	//create virtualaddress
	uint64_t phy_pml4e = allocate_page();
	proc->mm->pml4 = phy_pml4e;
	proc->mm->top_heap = 0;
	new_vir_pml4e = (uint64_t *)(VADDR + phy_pml4e);
	v_pml4=(uint64_t *)(VADDR + pml4);
	new_vir_pml4e[511]=v_pml4[511];
	
	for (i = 0; i < ehdr->e_phnum; ++i) 
	{
		if ((int)ph->p_type == 1) 
		{ 							// this is loadable section
			start_vaddr = ph->p_vaddr;
			size = ph->p_memsz;
			end_vaddr = start_vaddr + size;
			if (max_addr < end_vaddr) 
			{
				max_addr = end_vaddr;
			}
			
			if (ph->p_flags == 5) 
			{
				vm_type = TEXT;
				
			} else if (ph->p_flags == 6) 
			{
				vm_type = DATA;
				
			} else 
			{
				vm_type = NOTYPE;
				
			}

			struct vma *current_vma = make_vma(start_vaddr, end_vaddr, ph->p_type, vm_type, 0);	
			proc->mm->vma_count++;
			proc->mm->total_vm += size;
			current_vma->ph =  (uint64_t)ehdr + ph->p_offset;
			current_vma->mem_size = ph->p_memsz;
			
			nofpages=(size/4096);
			if(size%4096>0)
				nofpages=nofpages+1;


			// Load ELF sections into new Virtual Memory Area
			proc_paging(start_vaddr,nofpages+10 ,new_vir_pml4e);
			
			__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(proc->mm->pml4)); //set cur-PML4 in cr3
		
			memcpy((void*)start_vaddr,(void*)ehdr + ph->p_offset, (int64_t)(ph->p_filesz));
			memset((void *)start_vaddr + (int64_t)(ph->p_filesz), 0, size - (int64_t)(ph->p_filesz));
			

			// Load original PML4 
			__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pml4));

			// Add the current vma to the vma list in mm struct of task 
			add_vma_mm_list(current_vma, &proc->mm->vma_list);
						

			
		}
		ph=ph+1;
	
	}

	// Page Align the start and end address
	start_vaddr = end_vaddr = ((((max_addr - 1) >> 12) + 1) << 12);
	
	// Allocate vma for HEAP
	struct vma *heap_vma = make_vma(start_vaddr, end_vaddr, VMA_RW, HEAP, 0);
	add_vma_mm_list(heap_vma, &proc->mm->vma_list);
	proc->mm->vma_count++;
	proc->mm->start_brk = start_vaddr;
	proc->mm->end_brk = end_vaddr;	
	proc->mm->top_heap = 0;

	// Allocate vma for STACK
	end_vaddr = USER_STACK_TOP;
	start_vaddr = USER_STACK_TOP - USER_STACK_SIZE;
	struct vma *stack_vma = make_vma(start_vaddr, end_vaddr, VMA_RW, STACK, 0);
	add_vma_mm_list(stack_vma, &proc->mm->vma_list);

	__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(proc->mm->pml4)); 	
	proc_paging(end_vaddr-(512*8*10),10,new_vir_pml4e);		// Map single page of stack 
	__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pml4));
	proc->mm->start_stack = end_vaddr - 0x8;

	proc->mm->vma_count++;

// Copy arguments list to user stack
	  static char temp1[10][100];
	
	uint64_t *temp2[10],*stack;
	int argc = 0,len;
	strcpy(temp1[argc++], filename);
	
	if (argv) 
	{
		while (argv[argc-1]) 
		{
			strcpy(temp1[argc], argv[argc-1]);
			argc++;
		}
	}
	__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(proc->mm->pml4)); 
	stack = (uint64_t*)proc->mm->start_stack;

	for (i = argc-1; i >= 0; i--) 
	{
		len = strlen(temp1[i]) + 1;
		stack = (uint64_t*)((void*)stack - len);
		memcpy((char*)stack, temp1[i], len);
		temp2[i] = stack;
	}
	// storing pointers
	for (i = argc-1; i >= 0; i--) 
	{
		stack--;
		*stack = (uint64_t)temp2[i];
	}
	 int zz=9999;
	stack--;
	*stack = (uint64_t)argc;
	stack--;
	*stack=(uint64_t)zz;
	proc->mm->start_stack = (uint64_t)stack;
	__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pml4));


	// Initialize stack for user process
	proc->elf_start = ehdr->e_entry;
	
	initialize_stack(proc, ehdr->e_entry, proc->mm->start_stack,add);
	return proc;
}

void start_user_process(struct task *proc)
{
	struct elf64_ehdr* ehdr=(struct elf64_ehdr*) lookup(proc->name);
	schedule_my_task(proc, ehdr->e_entry, proc->mm->start_stack);
}
