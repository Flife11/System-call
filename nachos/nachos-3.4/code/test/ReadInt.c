#include "syscall.h"
#define MAX_BUFFER_LENGTH 255

int main() {

	PrintString("Nhap so nguyen: ");
	int a = 0;
	//a = ReadInt();


	PrintString("So nguyen vua nhap: ");
	PrintInt(a);
	PrintString("\n");


	Halt();
	return 0;
}

//#include "syscall.h"
//#include "copyright.h" 
//#define MAX_BUFFER_LENGTH 255
//
//int main() {
//
//	int a = 123;
//	PrintInt(a);
//	PrintString("\n");
//	Halt();
//	return 0;
//}

