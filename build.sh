gcc-10 main.c list_stack.c array_stack.c -g -o Debug.exe
gcc-10 main.c -L. -lstack -DNDEBUG -o Release.exe