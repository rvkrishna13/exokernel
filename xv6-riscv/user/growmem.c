// #include<kernel/syscall.h>
// #include "kernel/types.h"
// #include "user/user.h"

// int main(){
//     int size = 4096*256;
// 	printf("Before sbrk(%d):\n", size);
//     dump_page_table();
//     char *p = sbrk(size);
//     if (p == (char *)-1) {
//         printf("sbrk(%d) failed\n", size);
//         exit(1);
//     }for (int i = 0; i < size; i++) {
//         p[i] = 'A' + (i % 26); // Initialize with some meaningful data
//     }

//     // Access the memory and print its content

//     char a = 'a';
//     for (int i = 0; i < size; i++) {
//         p[i] = p[i]+i%26;
//     }
//     printf("one loop done\n");
//     for (int i = 0; i < size/2; i++) {
//         p[i] = p[i]+i%26;
//     }
//     dump_page_table();
//     printf("second loop done %c\n",a);
//     for (int i = 0; i < size/2; i++) {
//         p[i] = p[i]+i%26;
//     }
//     dump_page_table();
//     printf("third loop done %c\n",a);
//     printf("After sbrk(%d):\n", size);
//     printf("process pid is %d\n", getpid());
//     printf("dumped\n");
//     exit(0);
// }

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PAGE_SIZE 4096
#define NUM_PAGES 256


#define ARRAY_SIZE 10000

void helper(){
    int *array = (int *)sbrk(ARRAY_SIZE * sizeof(int));
    if (array == (int *)-1) {
        // printf("Memory allocation failed\n");
        exit(1);
    }
    dump_page_table();
    // Initialize the array with values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i;
    }
    int x=10;
    dump_page_table();
    // Print the array elements
    for (int i = 0; i < ARRAY_SIZE; i++) {
        // printf("array[%d] = %d\n", i, array[i]);
        x = array[i];
    }
    dump_page_table();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        x = array[i];
        // printf("array[%d] = %d\n", i, array[i]);
    }
    printf("%d x\n",x);
    x = x+10;
    dump_page_table();
}

int main() {
    dump_page_table();
    int pid = fork();
    // dump_page_table();
    if (pid < 0) {
        // fork() failed
        fprintf(1,"Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        // fprintf(1,"Child process created with PID: %d\n", getpid());
        dump_page_table();
        helper();
        exit(0);
    } else {
        // Parent process
        // printf("Parent process with PID: %d\n", getpid());
        dump_page_table();
        helper();
        wait(0);
        // printf("Child process finished\n");
    }
    helper();
    exit(0);
}