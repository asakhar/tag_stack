cd build

gcc-10 ../list_stack.c ../array_stack.c  -c
ar -rc libstack.a array_stack.o list_stack.o

cd ..

gcc-10 main.c list_stack.c array_stack.c -g -o build/Debug.exe
gcc-10 main.c -Lbuild/ -lstack -DNDEBUG -o build/Release.exe