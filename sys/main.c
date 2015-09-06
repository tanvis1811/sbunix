#include <sys/sbunix.h>
#include <sys/gdt.h>
#include <sys/tarfs.h>
#include <sys/memory.h>
#include <sys/paging.h>
#include <sys/task.h>
#include <sys/process.h>
#define INITIAL_STACK_SIZE 4096
#define kpages 1000
#define VADDR 0xFFFFFFFF80000000

extern void clear_screen();
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;
extern void idt_install();
extern void init_pic();
extern void init_timer();
extern void keyboard_install();
struct tss_t tss;
volatile int db = 1;

void start(uint32_t* modulep, void* physbase, void* physfree)
{
	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			printf("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
			
			init_phy(smap->base, smap->length, (uint64_t)physfree, (uint64_t)physbase);
		}
	}
	
	init_paging((uint64_t)&kernmem , (uint64_t) physbase, kpages);
		
	printf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
	

//	while(db);
	init_tarfs();
	load_elf("bin/idle",NULL,1);
	struct task* proc1 = load_elf("bin/sbush",NULL,1);
	load_elf("bin/idle",NULL,1);

	clear_screen();
	scheduling = 0;
	start_user_process(proc1);
	
	// kernel starts here
}

void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	//register char *s;
	__asm__(
		"movq %%rsp, %0;"
		"movq %1, %%rsp;"
		:"=g"(loader_stack)
		:"r"(&stack[INITIAL_STACK_SIZE])
	);
	
	reload_gdt();
	setup_tss();
	idt_install();
	init_pic();
	init_timer();
	keyboard_install();
	clear_screen();	
	
	
	start(
		(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
	
	while(1);
}
