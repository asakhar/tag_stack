#include "list_stack.h"
#include <malloc.h>
#include <errno.h>

void list_stack_pop(void *stack) {
  ListStack *ls = (ListStack *)stack;
  if (ls->top) {
    ListNode *ln = ls->top->prev;
    free(ls->top);
    ls->top = ln;
    return;
  }
  errno = EPERM;
}
void list_stack_push(void *stack, int val) {
  ListStack *ls = (ListStack *)stack;
  ListNode *new_node = (ListNode *)calloc(1, sizeof(ListNode));
  new_node->value = val;
  new_node->prev = ls->top;
  ls->top = new_node;
}
int list_stack_get(void *stack) {
  ListStack *ls = (ListStack *)stack;
  if (ls->top)
    return ls->top->value;
  errno = EPERM;
  return 0;
}

void clear_list_stack(void *stack) {
  while (errno == 0)
    list_stack_pop(stack);
  errno = 0;
}
void *create_list_stack() {
  ListStack *ls = (ListStack *)calloc(1, sizeof(ListStack));
  ls->top = 0;
  return (void *)ls;
}
void remove_list_stack(void *stack) {
  clear_list_stack(stack);
  free(stack);
}

int list_stack_empty(void* stack) {
  ListStack *ls = (ListStack *)stack;
  return ls->top==0;
}