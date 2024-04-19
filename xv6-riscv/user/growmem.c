#include<kernel/syscall.h>
#include "kernel/types.h"
#include "user/user.h"

int main(){
    dump_page_table();

    exit(0);
}