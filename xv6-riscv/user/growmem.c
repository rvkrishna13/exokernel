#include<kernel/syscall.h>
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/lib_os.h"

int main(){
    int size = 4096*10;
    int pid = 0;
	if(pid==0){
        printf("Before sbrk(%d):\n", size);
        dump_page_table();
        char *p = sbrk(size);
        if (p == (char *)-1) {
            printf("sbrk(%d) failed\n", size);
            exit(1);
        }for (int i = 0; i < size; i++) {
            p[i] = 'A' + (i % 26); // Initialize with some meaningful data
        }
        dump_page_table();
        // Access the memory and print its content
        // printf("trying to access memory after satp setting to 0\n");
        // char a = 'a';
        for (int i = 0; i < size; i++) {
            p[i] = p[i]+i%26;
        }
        // printf("one loop done\n");
        for (int i = 0; i < size/2; i++) {
            p[i] = p[i]+i%26;
        }
        // dump_page_table();
        // printf("second loop done %c\n",a);
        for (int i = 0; i < size/2; i++) {
            p[i] = p[i]+i%26;
        }
        dump_page_table();
        // printf("third loop done %c\n",a);
        // printf("After sbrk(%d):\n", size);
        // printf("process pid is %d\n", getpid());
        // printf("dumped\n");
        exit(0);
    }else{
        uint64 *ptr = (uint64 *) 0x55000;
        *ptr = 123;
        // uint a = *ptr;
        // *ptr = 123;
        printf("address %d %d\n", *ptr,123);
        wait(&pid);
    }
    
    exit(0);
}

     
