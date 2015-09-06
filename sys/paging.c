#include <sys/defs.h>
#include <sys/utilities.h>
#include <sys/sbunix.h>
#include <sys/memory.h>
#define VADDR 0xFFFFFFFF80000000
#define PAGESIZE 4096

extern void cr3 (uint64_t);
uint64_t *vir_pml4e = NULL;
uint64_t *pdpe = NULL, *pde = NULL, *pte = NULL;
 uint64_t *map_pte( int pde_off)
{
	uint64_t pte = allocate_page();
	pde[pde_off] = pte | 7;
	uint64_t *a = (uint64_t *) (VADDR + pte);
	return a;
}

 uint64_t *map_pde(int pdpe_off)
{
	uint64_t pde = allocate_page();
	pdpe[pdpe_off] = pde | 7;
	uint64_t *a = (uint64_t *) (VADDR + pde);
	return a;
}

 uint64_t *map_pdpe( int pml4e_off)
{
	uint64_t pdpe = allocate_page();
	vir_pml4e[pml4e_off] = pdpe | 7;
	uint64_t *a = (uint64_t *) (VADDR + pdpe);
	return a;
}

 void set_off(uint64_t vir_addr, int *pml4_off, int *pdpe_off, int *pde_off, int *pte_off)
{
	(*pml4_off) = (vir_addr >> 39) & 0x1FF;
	(*pdpe_off) = (vir_addr >> 30) & 0x1FF;
	(*pde_off) = (vir_addr >> 21) & 0x1FF;
	(*pte_off) = (vir_addr >> 12) & 0x1FF;
}

 void map_physical_virtual (uint64_t vaddr, uint64_t paddr, uint64_t *vir_pml4e, int no_of_pages)
{
	int i; 
	int pml4_off, pdpe_off, pde_off, pte_off;
	
	set_off(vaddr,&pml4_off, &pdpe_off, &pde_off, &pte_off);
	
	uint64_t phys_addr = (uint64_t) *(vir_pml4e + pml4_off);

	if (phys_addr & 1) 
	{
		phys_addr = phys_addr & 0xFFFFFFFFFFFFFF00;
		pdpe =(uint64_t*) (VADDR+phys_addr);
		phys_addr = (uint64_t) *(pdpe + pdpe_off);
		if (phys_addr & 1) 
		{
			phys_addr = phys_addr & 0xFFFFFFFFFFFFFF00;
			pde =(uint64_t*) (VADDR+phys_addr);
			phys_addr = (uint64_t) *(pde + pde_off);
			if (phys_addr & 1) 
			{
				phys_addr = phys_addr & 0xFFFFFFFFFFFFFF00;
				pte =(uint64_t*) (VADDR+phys_addr);
			} 
			else 
			{
				pte = map_pte( pde_off);
			}
		} 
		else 
		{
			pde = map_pde(  pdpe_off);
			pte = map_pte(  pde_off);
		}
	} 
	else 
	{
		pdpe = map_pdpe(  pml4_off);
		pde = map_pde(  pdpe_off);
		pte = map_pte(  pde_off);
	}
	phys_addr = paddr;
	if (no_of_pages + pte_off <= 512) 
	{
		for (i = pte_off; i < (pte_off + no_of_pages); i++) 
		{
			pte[i] = phys_addr | 7;
			phys_addr += PAGESIZE;
		}
		
	} 
}

uint64_t walk_find_paddr(uint64_t vaddr)
{
	int pml4_off, pdpe_off, pde_off, pte_off;
	set_off(vaddr,&pml4_off, &pdpe_off, &pde_off, &pte_off);
	
	uint64_t physaddr = (uint64_t) *(vir_pml4e + pml4_off);

	
	if(physaddr & 1)
	{	
		physaddr = physaddr & 0xFFFFFFFFFFFFFF00;
		pdpe =(uint64_t*) (VADDR+physaddr); 
		physaddr = (uint64_t) *(pdpe + pdpe_off);
	
		if(physaddr & 1)
		{
			physaddr = physaddr & 0xFFFFFFFFFFFFFF00;
			pde =(uint64_t*) (VADDR+physaddr);
			physaddr = (uint64_t) *(pde + pde_off);
	
			if(physaddr & 1)
			{
				physaddr = physaddr & 0xFFFFFFFFFFFFFF00;
				pte =(uint64_t*) (VADDR+physaddr);
				physaddr = (uint64_t) *(pte + pte_off);
				return physaddr;
			}	
		}
	}
	printf("Entry not present\n");
	return 0;
}

 void init_paging(uint64_t kernmem, uint64_t physbase, int no_pages)
{
	uint64_t phy_pml4e = allocate_page();
	vir_pml4e = (uint64_t *)(VADDR + phy_pml4e);
	
	/*	 Now we map kernel code starting from physbase to physfree plus 
			some extra memory specified by the no of pages	*/

	uint64_t j = 0;
	for(int i=0 ; i< no_pages; i++) 
	{
		map_physical_virtual (kernmem + j, physbase + j, vir_pml4e,1);
		j = j + 4096;
	}

	/* 	Mapping Video memory 	*/

	map_physical_virtual (0xFFFFFFFF800B8000, 0xB8000, vir_pml4e, 1);
	
	printf("Physical address corresponding to 0xFFFFFFFF80200000 is %x\n",walk_find_paddr(0xFFFFFFFF80200000));
	printf("Physical address corresponding to 0xFFFFFFFF800B8000 is %x\n",walk_find_paddr(0xFFFFFFFF800B8000));	

	/* 	Loading base address of PML4 into CR3 register	*/
	

	__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(phy_pml4e));
	//vir_pml4e[511] = phy_pml4e;
}

