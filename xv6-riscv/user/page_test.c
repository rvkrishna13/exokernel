#include "kernel/types.h"
#include "user/user.h"

#define SIZE 4096

void testing(){
	char *s = sbrk(SIZE*10);
	printf("After allocating 10 pages\n");
	print_nodes();
	printf("\n");
	for(int i=0; i < SIZE*10; i++){
		s[i] = 'A' + (i %26);
	}
	printf("After Accessing all 10 pages\n");
	print_nodes();
	printf("\n");
	
	sbrk(-5*SIZE);
	
	printf("After deallocating 5 pages\n");
	print_nodes();
	printf("\n");
	
	sbrk(-5*SIZE);
	
	printf("After deallocating remaining pages\n");
	print_nodes();
	printf("\n");
}

void main(){
	set_on_demand(1);
	set_va_pa_map(1);
	testing();
	set_va_pa_map(2);
	testing();
	
	exit(0);
}


