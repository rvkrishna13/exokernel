#include "types.h"

uint64 func1(){ // LIFO - Last In First Out
	struct pp_map *temp = myproc()->pp_map_node;
	struct user_va_pa_map_node *root = temp->map;
	while ((uint64)root->next != 0){
		root = root->next;
	}
	return root->va;
}

uint64 func2(){ // FIFO - First In First Out
	struct pp_map *temp = myproc()->pp_map_node;
	struct user_va_pa_map_node *root = temp->map;
	if((uint64)root == 0){
		return 0;
	}
	uint64 xx = root->va;
	return xx;
}

uint64 (*func_list[])(void) = {func1, func1, func2};
