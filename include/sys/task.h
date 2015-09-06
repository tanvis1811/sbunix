#include <sys/defs.h>
typedef uint32_t pid_t;
#define IDLE 1
#define READY 2
#define WAITING 3

#define TEXT 1
#define DATA 2
#define NOTYPE 3
#define HEAP 4
#define STACK 5

#define VMA_R 1
#define VMA_RX 2
#define VMA_RW 3
#define VMA_RWX 2

#define USER_STACK_TOP 0xF000000000
#define USER_STACK_SIZE 10240

struct task *running;
volatile int scheduling;
uint64_t pml4;
struct vma 
{
	struct mm_struct *vm_mm; // The address space we belong to.
	uint64_t vm_start; // Our start address within vm_mm
	uint64_t vm_end; // The first byte after our end address within vm_mm
	uint64_t vm_flags; // Flags read, write, execute permissions
	uint64_t vm_type; // type of segment its reffering to
	uint64_t vm_file_descp; // reference to file descriptors for file opened for writing
	uint64_t ph;
	uint64_t mem_size;
	struct vma *next; // linked list of VM areas per task, sorted by address
};

struct mm_struct
{
	struct vma *vma_list; // list of VMAs
	uint64_t pml4; // Actual physical base addr for PML4 table
	uint32_t vma_count; // number of VMAs
	uint64_t hiwater_vm; // High-water virtual memory usage
	uint64_t total_vm;
	uint64_t stack_vm;
	uint64_t start_brk, end_brk, start_stack;
	uint64_t arg_start, arg_end, env_start, env_end;
	uint64_t top_heap;
};

struct task 
{
	int task_type;
	int task_state;
	uint64_t pid;	// Process ID
	uint64_t ppid;	// Process ID	uint64_t ppid;	// Process ID
	uint64_t pml4e;	// Entry of process in pml4e table
	uint64_t u_stack[64];	// Process ID
	struct task *prev;
	struct task *next;
	struct mm_struct *mm;	// Pointer to First VMA Block of the Process 
	uint64_t cr3;	// Contents of CR3 Register. (add of PML4E page table)
	uint64_t proc_present;	// Value of proc_present bit will tell how many processes with current process as parent are present 
	uint64_t start_time;
	char name[100];
	uint64_t sleep_start;	
	uint64_t sleep_duration;	
	uint64_t rip;
	uint64_t rsp;	
	uint64_t elf_start;
	uint64_t kernel_stack[512];	
	int sleep;
	int time_to_sleep;
};

struct task *make_task(int process_type,char *name);
struct vma *make_vma(uint64_t start, uint64_t end, uint64_t flags, uint64_t type, uint64_t fd);
void schedule_my_task(struct task *current, uint64_t start, uint64_t top_of_stack);
void initialize_stack1();
void initialize_stack(struct task *proc, uint64_t start, uint64_t top_of_stack,int add);
void scheduler();
void start_executing();
int fork();
int getpid();
int getppid();
int execve(char *filename, char * argv[], char * envp[]);
void make_new_running_proc( struct task* new_proc);
uint64_t ksbrk(uint64_t size);
int exit();
int waitpid();
uint64_t ksbrk(uint64_t size);

//struct task *fork (struct task *current);
