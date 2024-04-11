#include<kernel/syscall.h>
#include "kernel/types.h"
#include "user/user.h"

int main(){
    int size = 4096*4;
	printf("Before sbrk(%d):\n", size);
    dump_page_table();
    char *p = sbrk(size);
    if (p == (char *)-1) {
        printf("sbrk(%d) failed\n", size);
        exit(1);
    }for (int i = 0; i < size; i++) {
        p[i] = 'A' + (i % 26); // Initialize with some meaningful data
    }

    // Access the memory and print its content
    for (int i = 0; i < size; i+=1000) {
        printf("Accessing %c from the new memory\n", p[i]);
    }
    printf("accessing %c new memory\n", *p);
    printf("After sbrk(%d):\n", size);
    dump_page_table();

    exit(0);
}