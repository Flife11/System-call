#include "syscall.h"

int main()
{
	int n;	
	int printAID, printBID;
	int flag;
	
//	while (1) {
		PrintString("Chon chuong trinh ban muon chay: \n");
		PrintString("1: In a, b dong thoi\n");
		PrintString("2: In a, b dong thoi ket hop semaphore\n");
		PrintString("3: Exit\n");
		n = ReadInt();
		PrintInt(n);
		switch(n) {
			case 1: {
				PrintString("Ping-pong test 1 bat dau: ....\n");	
		
				flag = CreateSemaphore("print_f", 0);
				if (flag==-1)
				flag = CreateSemaphore("print_c", 1000);
				if (flag==-1) return 1;

				printAID = Exec("./test/printa");
				printBID = Exec("./test/printb");
				Join(printAID);
				Join(printBID);
				break;
			}
			case 2: {
				PrintString("Ping-pong test 2 bat dau: ....\n");

				flag = CreateSemaphore("print_f", 0);
				if (flag==-1) return 1;
				flag = CreateSemaphore("print_c", 1);
				if (flag==-1) return 1;

				printAID = Exec("./test/printa");
				printBID = Exec("./test/printb");				
				Join(printAID);
				Join(printBID);
				break;
			}
			default: {
				Exit(0);
				break;
			}				
		}
	//}
	
	Halt();
}
