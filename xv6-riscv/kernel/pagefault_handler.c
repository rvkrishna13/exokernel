#include "param.h"
#include "types.h"
#include "memlayout.h"
#include "elf.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "spinlock.h"
#include "proc.h"

int pagefault_handler(void)
{
	uint64 va = r_stval();  // Get the faulting virtual address from stval
    if(va==0)
      return 1;
    // pte_t *pte = NULL;
    struct proc *p = myproc();
    // Traverse the page table to find the corresponding PTE
    // if(p->pid>=3)
    //   pte = walk(p->pagetable, va, 0);
    // else
    //   pte = walk(p->pagetable, va, 0);
    char *pa;
    pa = kalloc();
    memset(pa, 0, PGSIZE);
    mappages(p,p->pagetable, va, PGSIZE, (uint64)pa,  PTE_R|PTE_W|PTE_U);
    print_page_table();
    
    // Communicate the updated PTE to the hardware TLB
    asm volatile("sfence.vma %0" : : "r" (va));
    
    return 1;
}