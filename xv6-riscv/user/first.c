#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/memlayout.h"
#include "kernel/spinlock.h"
#include "kernel/riscv.h"
#include "kernel/lib_os.h"
#include "kernel/proc.h"
#include "kernel/defs.h"

void printing_g(){
	printf("Hello World\n");
}

void main(void) {
	printing_g();
	//printf("HHHHHHh 0000000000000000000000000\n");
	//int a = 10;
	//printf("wejkfbwef %d\n",a);
	//struct pp_map *ppp = (struct pp_map *)(uint64)myproc();
	//ppp->func = jkl;
	exit(0);
}


