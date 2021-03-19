gcc.exe main.c list_stack.c array_stack.c -g -o Debug.exe
gcc.exe main.c -L. -lstack -DNDEBUG -o Release.exe