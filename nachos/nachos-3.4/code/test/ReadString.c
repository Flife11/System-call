#include "syscall.h"
#define MAX_BUFFER_LENGTH 254

int main() {

	char* buffer[MAX_BUFFER_LENGTH - 1];
    
	PrintString("Nhap chuoi: ");
	ReadString(buffer, MAX_BUFFER_LENGTH);

	PrintString("Chuoi vua nhap: ");
	PrintString(buffer);
	PrintString("\n");	


	Halt();
}
