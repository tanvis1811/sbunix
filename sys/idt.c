#include <sys/defs.h>
#include <sys/utilities.h>
#include <sys/sbunix.h>
#define MAX_IDT 256

struct idt_entry
{
	uint16_t offset_low;     
	uint16_t selector;       
	unsigned char ist;       
	unsigned char type_attr; 
	uint16_t offset_middle;  
	uint32_t offset_high;    
	uint32_t zero;           
}  __attribute__((packed));
 
struct idtr_t
{
	uint16_t size;
	uint64_t addr;
} __attribute__((packed));

struct idt_entry idt[MAX_IDT];
struct idtr_t idtp;

extern void idt_load(struct idtr_t *idtp);
extern void isr0();
extern void irq0();
extern void irq1();
extern void isr14();
extern void isr80();

void idt_set_gate(unsigned char num, uint64_t base, uint16_t selector, unsigned char flags)
{
	idt[num].offset_low = (base & 0xFFFF);
	idt[num].offset_middle = ((base>>16) & 0xFFFF);
	idt[num].offset_high = ((base>>32) & 0xFFFFFFFF);
	idt[num].selector = selector;
	idt[num].ist = 0;
	idt[num].type_attr= flags;
}

void idt_install()
{
	idtp.size = sizeof(idt);
	idtp.addr = (uint64_t)idt;
	idt_set_gate(0, (uint64_t)isr0, 0x08, 0x8E);
	idt_set_gate(14, (uint64_t)isr14, 0x08, 0x8E);
	idt_set_gate(32, (uint64_t)irq0, 0x08, 0x8E);
	idt_set_gate(33, (uint64_t)irq1, 0x08, 0x8E);
	idt_set_gate(128, (uint64_t)isr80, 0x08, 0xEE);
	idt_load(&idtp);
}
