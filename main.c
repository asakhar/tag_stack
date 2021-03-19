#include "array_stack.h"
#include "list_stack.h"
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

size_t sstrlen(char const *str, size_t max_length) {
  size_t i = 0;
  for (; str[i] && i < max_length; ++i)
    ;
  return i;
}

int get_int(char *str, size_t max_size) {
  char *end = 0;
  long num = strtol(str, &end, 10);
  if ((size_t)labs(end - str) == sstrlen(str, max_size) - 1 && num <= INT32_MAX)
    return (int)num;
  errno = EOVERFLOW;
  return 0;
}

void itoa(int *begin, int *end, int start_val) {
  for (; begin != end; ++begin)
    *begin = start_val++;
}

int make_move(int *tag, int move) {
  int zero_place = 0;
  for (; zero_place < 16; ++zero_place)
    if (tag[zero_place] == 0)
      break;
  if (zero_place / 4 != 0 && tag[zero_place - 4] == move) {
    tag[zero_place] = tag[zero_place - 4];
    tag[zero_place - 4] = 0;
    return 0;
  }
  if (zero_place / 4 != 3 && tag[zero_place + 4] == move) {
    tag[zero_place] = tag[zero_place + 4];
    tag[zero_place + 4] = 0;
    return 0;
  }
  if (zero_place % 4 != 0 && tag[zero_place - 1] == move) {
    tag[zero_place] = tag[zero_place - 1];
    tag[zero_place - 1] = 0;
    return 0;
  }
  if (zero_place % 4 != 3 && tag[zero_place + 1] == move) {
    tag[zero_place] = tag[zero_place + 1];
    tag[zero_place + 1] = 0;
    return 0;
  }
  return 1;
}

void draw(int *tag) {
  printf("\n\n\n");
  for (int i = 0; i < 16; ++i) {
    if (tag[i] == 0) {
      printf("   #");
      goto endl;
    }
    if (tag[i] > 9) {
      printf("  %d", tag[i]);
      goto endl;
    } else {
      printf("   %d", tag[i]);
    }
  endl:
    if (i % 4 == 3)
      printf("\n\n");
  }
}

void shuffle_tag(int *tag, int number_of_moves) {
  int last = 0;
  while (number_of_moves != 0) {
    int move = rand() % 15 + 1;
    if (last != move && !make_move(tag, move)) {
      last = move;
      number_of_moves--;
#ifndef NDEBUG
      draw(tag);
      struct timespec req, rem;
      req.tv_sec = 0;
      req.tv_nsec = 500000000ll;
      nanosleep(&req, &rem);
#endif
    }
  }
}

void pour_over(int *tag, void *stack1, void *stack2, int mode) {
  int val;
  if (mode) {
    val = array_stack_get(stack1);
    if (make_move(tag, val))
      puts("Error happened!\n");
    array_stack_push(stack2, val);
    array_stack_pop(stack1);
  } else {
    val = list_stack_get(stack1);
    if (make_move(tag, val))
      puts("Error happened!\n");
    list_stack_push(stack2, val);
    list_stack_pop(stack1);
  }
}

int main(void) {
  int mode;
  puts("Enter stack mode (L - for list, A - for array):\n");
  while (1) {
    char *str = 0;
    size_t size = 0;
    getline(&str, &size, stdin);
    if (sstrlen(str, size) != 2) {
      puts("Try again:\n");
      continue;
    }
    if (tolower(str[0]) == 'l') {
      mode = 0;
      break;
    }
    if (tolower(str[0]) == 'a') {
      mode = 1;
      break;
    }
    puts("Try again:\n");
  }
  size_t array_size;
  if (mode)
    puts("Enter array capacity:\n");
  while (mode) {
    char *str = 0;
    size_t size = 0;
    getline(&str, &size, stdin);
    if ((array_size = get_int(str, size)) && errno == 0)
      break;
    errno = 0;
    puts("Try again:\n");
  }
  void *stack;
  void *forward_stack;
  if (mode) {
    stack = create_array_stack(array_size);
    forward_stack = create_array_stack(array_size);
  } else {
    stack = create_list_stack();
    forward_stack = create_list_stack();
  }
  size_t shuffle_path_length;
  puts("Enter number of moves to shuffle:\n");
  while (1) {
    char *str = 0;
    size_t size = 0;
    getline(&str, &size, stdin);
    shuffle_path_length = get_int(str, size);
    if (errno == 0)
      break;
    errno = 0;
    puts("Try again:\n");
  }
  int tag[16];
  itoa(tag, tag + 16, 0);
  shuffle_tag(tag, shuffle_path_length);

  while (1) {
    draw(tag);

    char *str = 0;
    size_t size = 0;
    getline(&str, &size, stdin);
    int move = get_int(str, size);
    if (errno == 0) {
      if (make_move(tag, move))
        puts("Illegal move.\n");
      else {
        if (mode) {
          array_stack_push(stack, move);
          clear_array_stack(forward_stack);
        } else {
          list_stack_push(stack, move);
          clear_list_stack(forward_stack);
        }
      }
      continue;
    }
    errno = 0;
    if (sstrlen(str, size) != 2) {
      puts("Try again:\n");
      continue;
    }
    if (tolower(str[0]) == 'u') {
      if (mode ? array_stack_empty(stack) : list_stack_empty(stack)) {
        puts("No more moves to undo.\n");
        continue;
      }
      pour_over(tag, stack, forward_stack, mode);
      continue;
    }
    if (tolower(str[0]) == 'r') {
      if (mode ? array_stack_empty(forward_stack)
               : list_stack_empty(forward_stack)) {
        puts("No more moves to redo.\n");
        continue;
      }
      pour_over(tag, forward_stack, stack, mode);
      continue;
    }
    if (tolower(str[0]) == 's') {
      puts("Enter number of moves to shuffle:\n");
      while (1) {
        char *str = 0;
        size_t size = 0;
        getline(&str, &size, stdin);
        shuffle_path_length = get_int(str, size);
        if (errno == 0)
          break;
        errno = 0;
        puts("Try again:\n");
      }
      shuffle_tag(tag, shuffle_path_length);
      continue;
    }
    if (tolower(str[0]) == 'e') {
      exit(0);
    }
    puts("Invalid option!\n");
  };
  return 0;
}
