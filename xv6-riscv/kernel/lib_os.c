#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "lib_os.h"
#include "proc.h"
#include "defs.h"

void write_to_buffer(struct proc *p, uint64 va){
	struct buffer_node *temp = p->buf;
	while((uint64)temp->va != 0){
		temp = temp->next;
	}
	if ((uint64)temp != 0){
		int x = copyin(p, p->pagetable, temp->content, va, PGSIZE);
		if (x == 0){
			temp->va = va;
			printf("Successful copy to buffer for va %d\n", va);
		} else {
			printf("some problem on va %d %d\n", va, x);
		}
	}
}

uint64 ghi(){
	struct pp_map *temp = myproc()->pp_map_node;
	struct user_va_pa_map_node *root = temp->map;
	while ((uint64)root->next != 0){
		root = root->next;
	}
	return root->va;
}

void recursive_del(struct user_va_pa_map_node *temp){
	if((uint64)temp != 0){
		if((uint64)temp->next != 0){
			recursive_del(temp->next);
		}
		kfree(temp);
	}
}

void recurs_del_buffer(struct buffer_node *temp){
	if((uint64)temp != 0){
		if((uint64)temp->next !=0){
			recurs_del_buffer(temp->next);
		}
		kfree(temp->content);
		kfree(temp);
	}
}

void del_map(uint64 pid){
	struct pp_map *temp = myproc()->pp_map_node;
	recursive_del(temp->map);
	kfree(temp);
	recurs_del_buffer(myproc()->buf);
}

struct user_va_pa_map_node *create_new_va_pa_map_node(){
	struct user_va_pa_map_node *temp;
	temp = (struct user_va_pa_map_node *)kalloc();
	return temp;
}

void add_va_pa_map(uint64 va, uint64 pa){
	struct pp_map *p = myproc()->pp_map_node;
	struct user_va_pa_map_node *new_node = create_new_va_pa_map_node();
	new_node->va = va;
	new_node->pa = pa;
	new_node->next = 0;
	printf("Adding Map for pid %d for va %d and pa %d\n", p->pid, new_node->va, new_node->pa);
	p->size++;
	if ((uint64)p->map == 0){
		p->map = new_node;
		return;
	}
	struct user_va_pa_map_node *root = p->map;
	while((uint64)root->next != 0){
		root = root->next;
	}
	root->next = new_node;
	//print_all_user_map();
}

void rm_node_user(struct pp_map *user, uint64 va){
	struct user_va_pa_map_node *root = user->map;
	struct user_va_pa_map_node *prev = 0;
	while((uint64)root !=0 && root->va != va){
		prev = root;
		root = root->next;
	}
	if(prev == 0){
		user->map = user->map->next;
	}else{
		prev->next = root->next;
	}
	//printf("Deleting for map node for va %d %p\n", root->va);
	kfree(root);
}

void try_alloc_page(){
	struct proc *pp = myproc();
	struct pp_map *user_node = pp->pp_map_node;
	if (user_node->size >= MAX_PHY_PAGES){
		uint64 remove_va = user_node->func();
		//printf("Trying remove_va %p %d\n", remove_va, remove_va);
		if ((uint)remove_va != 0){
			write_to_buffer(pp, remove_va);
			uvmunmap(pp->pagetable, remove_va, 1, 1);
			rm_node_user(user_node, remove_va);
			asm volatile("sfence.vma %0" : : "r"(remove_va));
			user_node->size--;
		}
	}
}

struct buffer_node *get_buffer_node(struct buffer_node *temp, uint64 va){
	while((uint64)temp != 0){
		if(temp->va == va){
			return temp;
		}
		temp = temp->next;
	}
	return 0;
}

struct buffer_node *get_empty_node(struct buffer_node *temp){
	while((uint64)temp != 0){
		if(temp->va == 0){
			return temp;
		}
		temp = temp->next;
	}
	return 0;
}

void swap_buffer(uint64 va){
	struct proc *pp = myproc();
	struct pp_map *user_node = pp->pp_map_node;
	if (user_node->size >= MAX_PHY_PAGES){
		uint64 remove_va = user_node->func();
		if ((uint)remove_va != 0){
			printf("swaping va %d with remove_va %d\n", va, remove_va);
			char *paa;
			paa = kalloc();
			memset(paa, 0, PGSIZE);
			newmappages(pp->pagetable, va, (uint64)paa, PTE_R|PTE_U|PTE_W);
			struct buffer_node *temp = get_buffer_node(pp->buf, va);
			copyout(pp, pp->pagetable, va, temp->content, PGSIZE);
			temp->va = 0;
			
			struct buffer_node *emp = get_empty_node(pp->buf);
			copyin(pp,pp->pagetable, emp->content, remove_va, PGSIZE);

			emp->va = remove_va;
			uvmunmap(pp->pagetable, remove_va, 1, 1);
			rm_node_user(user_node, remove_va);
			asm volatile("sfence.vma %0" : : "r"(remove_va));
			asm volatile("sfence.vma %0" : : "r"(va));
			return;
		}
	}
}



