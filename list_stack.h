#ifndef LIST_STACK_H
#define LIST_STACK_H

struct ListNode {
  int value;
  struct ListNode* prev;
};

typedef struct ListNode ListNode;

struct ListStack {
  ListNode *top;
};
typedef struct ListStack ListStack; 

void list_stack_pop(void* stack);
void list_stack_push(void* stack, int val);
int list_stack_get(void* stack);
void clear_list_stack(void* stack);
void* create_list_stack();
void remove_list_stack(void* stack);
int list_stack_empty(void* stack);

#endif // LIST_STACK_H
