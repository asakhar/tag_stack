gcc.exe main.c list_stack.c array_stack.c -g -o build/Debug.exe
gcc.exe main.c -Lbuild/ -lstack -DNDEBUG -o build/Release.exe