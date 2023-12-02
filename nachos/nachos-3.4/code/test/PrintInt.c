#include "syscall.h"
#include "copyright.h" 
#define MAX_BUFFER_LENGTH 255

int main() {

    int a = 123;
    PrintInt(a);
    PrintString("\n");
    Halt();
    return 0;
}
