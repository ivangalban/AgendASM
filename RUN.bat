nasm -f win32 Agenda.asm
gcc -c -m32 Tokenizing.c
gcc Agenda.obj Tokenizing.o -m32 -o Agenda.exe