#ifndef ARRAY_STACK_H
#define ARRAY_STACK_H

#include <stddef.h>

struct ArrayStack {
  int *values;
  size_t size;
  int top;
  size_t current_size;
};
typedef struct ArrayStack ArrayStack;

void array_stack_pop(void *stack);
void array_stack_push(void *stack, int val);
int array_stack_get(void *stack);
void clear_array_stack(void *stack);
void *create_array_stack(size_t size);
void remove_array_stack(void *stack);
int array_stack_empty(void* stack);

#endif // ARRAY_STACK_H
