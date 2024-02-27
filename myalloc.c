#include "myalloc.h"
#include <stddef.h> //NULL
#include <sys/mman.h>
#include <stdio.h>

void * heap = NULL;

void *myalloc(int size){
  //Heap initialization
  if(heap == NULL){
    heap = init_heap();
  }

  //the minimum size for a split, requested size + header size
  int minimum_split_size = PADDED_SIZE(size) + PADDED_SIZE(sizeof(struct block)) + 16;
  int minimum_size = PADDED_SIZE(size);

  struct block * block = heap;
  while(block != NULL){
    //in use check
    if(block->in_use){
      block = block->next;
    }else{
      //checking size
      if(block->size < minimum_size){//below minimum
        block = block->next;
      }else if(block->size < minimum_split_size){//below split size
        block->in_use = 1;
        return block + PADDED_SIZE(sizeof(struct block));
      }else{//above split size
        block->in_use = 1;
        return split_block(block, size) + PADDED_SIZE(sizeof(struct block));
      }
    }
  }

  return NULL;
}

void *init_heap(){
  void * heap_mem = mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
  struct block * head = heap_mem;
  head->size = 1024 - PADDED_SIZE(sizeof(struct block));
  head->in_use = 0;
  head->next = NULL;
  return heap_mem;
}

void *split_block(struct block *block, int size){
  int old_size = block->size - (PADDED_SIZE(size) + PADDED_SIZE(sizeof(struct block)));
  block->size = PADDED_SIZE(size);
  struct block *new_block = PTR_OFFSET(block, PADDED_SIZE(size));
  new_block->size = old_size;
  new_block->in_use = 0;
  new_block->next = block->next;
  block->next = new_block;
  return block;
}

void myfree(void *p){
  int neg_offset = -1 * (int)PADDED_SIZE(sizeof(struct block));
  struct block *block_to_free = PTR_OFFSET(p, neg_offset);
  block_to_free->in_use = 0;

  merge_free();
}

void merge_free(){
  struct block *current = heap;
  struct block *next = NULL;

  while(current->next != NULL){
    //if current is not in use check next node for merging, else get next
    if(!current->in_use){
      //get next and check it before utilizing it
      next = current->next;
      if(!next->in_use){
        int merge_size = current->size + next->size + PADDED_SIZE(sizeof(struct block));
        //get the furtherest chained free node
        while(next->next != NULL && !next->next->in_use){//short circuit logic
          next = next->next;
          merge_size += next->size + PADDED_SIZE(sizeof(struct block));
        }
        //preform merge
        current->size = merge_size;
        if(next->next != NULL){//only re-link the current->next if next is not the end of the list
          current->next = next->next;
        }else{
          current->next = NULL;
          break;//normal while loop logic ends since there will be no next node
        }
      }
    }
    current = current->next;
  }


}

void print_data(void)
{
  struct block *b = heap;

  if (b == NULL) {
    printf("[empty]\n");
    return;
  }

  while (b != NULL) {
    // Uncomment the following line if you want to see the pointer values
    //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
    printf("[%d,%s]", b->size, b->in_use? "used": "free");
    if (b->next != NULL) {
      printf(" -> ");
    }

    b = b->next;
  }

  printf("\n");
}
