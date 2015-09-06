#include <sys/utilities.h>

 uint64_t* map_pte(int pde_off,uint64_t *pde);
 uint64_t* map_pde(int pdpe_off,uint64_t *pdpe);
 uint64_t* map_pdpe(int pml4e_off,uint64_t *pml4e);
 uint64_t *vmap_pdpe( int pml4e_off,uint64_t* vir_pml4e);
 uint64_t *vmap_pde(int pdpe_off,uint64_t* pdpe);
 uint64_t *vmap_pte( int pde_off,uint64_t* pde);

void init_paging(uint64_t kernmem, uint64_t physbase, int kpages);
 void vset_off(uint64_t vir_addr, int *pml4_off, int *pdpe_off, int *pde_off, int *pte_off);

 void map_physical_virtual (uint64_t vaddr, uint64_t paddr, uint64_t *vir_pml4e, int no_pages);
 void vmap_physical_virtual (uint64_t vaddr, uint64_t paddr, uint64_t *new_vir_pml4e, int no_of_pages);
 void set_off(uint64_t vir_addr, int *pml4_off, int *pdpe_off, int *pde_off, int *pte_off);
void proc_paging(uint64_t start_vaddr,int no_pages,uint64_t* vir_pml4e);
uint64_t new_walk_find_paddr(uint64_t vaddr,uint64_t* new_vir_pml4e);