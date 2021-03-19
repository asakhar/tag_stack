#include "array_stack.h"
#include "malloc.h"
#include <errno.h>

void array_stack_pop(void *stack) {
  ArrayStack *as = (ArrayStack *)stack;
  if (as->current_size != 0) {
    if (as->top == 0)
      as->top = as->size;
    --as->top;
    --as->current_size;
    return;
  }
  errno = EPERM;
}
void array_stack_push(void *stack, int val) {
  ArrayStack *as = (ArrayStack *)stack;
  if (as->current_size < as->size)
    ++as->current_size;
  as->values[as->top] = val;
  as->top = (as->top + 1) % as->size;
}
int array_stack_get(void *stack) {
  ArrayStack *as = (ArrayStack *)stack;
  if (as->current_size != 0)
    return as->values[as->top != 0 ? as->top - 1 : as->size - 1];
  errno = EPERM;
  return 0;
}

void clear_array_stack(void *stack) {
  ArrayStack *as = (ArrayStack *)stack;
  as->top = 0;
  as->current_size = 0;
}
void *create_array_stack(size_t size) {
  ArrayStack *as = (ArrayStack *)calloc(1, sizeof(ArrayStack));
  as->size = size;
  as->current_size = 0;
  as->top = 0;
  as->values = (int *)calloc(size, sizeof(int));
  return (void *)as;
}

void remove_array_stack(void *stack) {
  ArrayStack *as = (ArrayStack *)stack;
  free(as->values);
  free(as);
}

int array_stack_empty(void* stack) {
  ArrayStack *as = (ArrayStack *)stack;
  return as->current_size == 0;
}