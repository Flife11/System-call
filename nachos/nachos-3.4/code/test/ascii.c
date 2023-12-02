#include "syscall.h"
#include "copyright.h"

//khoang gia tri ASCII co the hien thi tren console
#define MIN_ASCII 32
#define MAX_AsCII 126
int main()
{
    int i;
    PrintString("ASCII table\n");
    for (i = MIN_ASCII; i <= MAX_AsCII; i++)
    {
        PrintInt(i);       //in giá trị decimal
        PrintChar('-');
        PrintChar((char)i); //in kí tự
        PrintChar('\n');
    }
    Halt();
    return 0;
}
