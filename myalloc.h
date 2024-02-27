#ifndef MYALLOC_H
#define MYALLOC_H

#define ALIGNMENT 16
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))

#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

struct block {
  int in_use;
  int size;
  struct block * next;
};

void *myalloc(int size);
void *init_heap();
void *split_block(struct block *block, int size);

void myfree(void *p);
void merge_free();

void print_data(void);

#endif
