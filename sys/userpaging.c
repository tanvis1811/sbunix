#include <sys/defs.h>
#include <sys/utilities.h>
#include <sys/sbunix.h>
#include <sys/memory.h>
#define VADDR 0xFFFFFFFF80000000
#define PAGESIZE 4096


 uint64_t *vmap_pte( int pde_off,uint64_t* pde)
{
	uint64_t pte = allocate_page();
	pde[pde_off] = pte | 7;
	uint64_t *a = (uint64_t *) (VADDR + pte);
	return a;
}

 uint64_t *vmap_pde(int pdpe_off,uint64_t* pdpe)
{
	uint64_t pde = allocate_page();
	pdpe[pdpe_off] = pde | 7;
	uint64_t *a = (uint64_t *) (VADDR + pde);
	return a;
}

 uint64_t *vmap_pdpe( int pml4e_off,uint64_t* vir_pml4e)
{
	uint64_t pdpe = allocate_page();
	vir_pml4e[pml4e_off] = pdpe | 7;
	uint64_t *a = (uint64_t *) (VADDR + pdpe);
	return a;
}

 void vset_off(uint64_t vir_addr, int *pml4_off, int *pdpe_off, int *pde_off, int *pte_off)
{
	(*pml4_off) = (vir_addr >> 39) & 0x1FF;
	(*pdpe_off) = (vir_addr >> 30) & 0x1FF;
	(*pde_off) = (vir_addr >> 21) & 0x1FF;
	(*pte_off) = (vir_addr >> 12) & 0x1FF;
}

 void vmap_physical_virtual (uint64_t vaddr, uint64_t paddr, uint64_t *new_vir_pml4e, int no_of_pages)
{   
	int i; 
	int pml4_off, pdpe_off, pde_off, pte_off;
	uint64_t* pde;
	uint64_t* pte;
	uint64_t* pdpe;
	
	vset_off(vaddr,&pml4_off, &pdpe_off, &pde_off, &pte_off);
	
	uint64_t phys_addr = (uint64_t) *(new_vir_pml4e + pml4_off);

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
				pte = vmap_pte( pde_off,pde);
			}
		} 
		else 
		{
			pde = vmap_pde(  pdpe_off,pdpe);
			pte = vmap_pte(  pde_off,pde);
		}
	} 
	else 
	{
		pdpe = vmap_pdpe(  pml4_off,new_vir_pml4e);
		pde = vmap_pde(  pdpe_off,pdpe);
		pte = vmap_pte(  pde_off,pde);
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

uint64_t new_walk_find_paddr(uint64_t vaddr,uint64_t* vir_pml4e )
{
	int pml4_off, pdpe_off, pde_off, pte_off;
	vset_off(vaddr,&pml4_off, &pdpe_off, &pde_off, &pte_off);
	uint64_t* pde;
	uint64_t* pte;
	uint64_t* pdpe;
	
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



 

	


void proc_paging(uint64_t start_vaddr,int no_pages,uint64_t* new_vir_pml4e)
{
	
	uint64_t j = 0;
	for(int i=0 ; i< no_pages; i++) 
	{ 
		uint64_t physpage=allocate_page();

		vmap_physical_virtual (start_vaddr + j, physpage, new_vir_pml4e,1);

			
		j = j + 4096;
	}
}
