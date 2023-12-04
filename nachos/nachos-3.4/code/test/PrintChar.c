#include "syscall.h"

int main() {

	char ch;
    
	PrintString("Nhap ky tu bat ki: ");
	ch = ReadChar();

	PrintString("Ky tu vua nhap: ");
	PrintChar(ch);
	PrintString("\n");	


	Halt();
}
