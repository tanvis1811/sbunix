#include <sys/utilities.h>
#include <sys/elf.h>

struct mm_struct* create_new_mmstruct();
struct task* create_new_task(char *filename);
int check_elf(Elf64_Ehdr* hdr);
struct task *load_elf(char *filename, char *argv[],int add);
void start_user_process(struct task *proc);
void add_vma_mm_list(struct vma *current_vma, struct task **proc);
