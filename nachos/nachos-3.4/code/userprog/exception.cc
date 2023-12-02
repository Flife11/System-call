// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#define MAX_BUFFER_LENGTH 255

/*
Input: - User space address (int)
- Limit of buffer (int)
Output:- Buffer (char*)
Purpose: Copy buffer from User memory space to System memory space
*/
char *User2System(int virtAddr, int limit)
{
    int i; // index
    int oneChar;
    char *kernelBuf = NULL;
    kernelBuf = new char[limit + 1]; //need for terminal string
    if (kernelBuf == NULL)
        return kernelBuf;
    memset(kernelBuf, 0, limit + 1);
    // for (int i = 0; i < limit+1; ++i) {
    //     kernelBuf[i] = '\0';
    // }
    //printf("\n Filename u2s:");
    for (i = 0; i < limit; i++)
    {
        machine->ReadMem(virtAddr + i, 1, &oneChar);
        kernelBuf[i] = (char)oneChar;
        //printf("%c",kernelBuf[i]);
        if (oneChar == 0)
            break;
    }
    return kernelBuf;
}
/*
Input: - User space address (int)
- Limit of buffer (int)
- Buffer (char[])
Output:- Number of bytes copied (int)
Purpose: Copy buffer from System memory space to User memory space
*/
int System2User(int virtAddr, int len, char *buffer)
{
    if (len < 0)
        return -1;
    if (len == 0)
        return len;
    int i = 0;
    int oneChar = 0;
    do
    {
        oneChar = (int)buffer[i];
        machine->WriteMem(virtAddr + i, 1, oneChar);
        i++;
    } while (i < len && oneChar != 0);
    return i;
}


// hàm tăng giá trị thanh ghi PC
void IncreasePC()
{
	int counter = machine->ReadRegister(PCReg); //lưu giá trị thanh ghi lệnh hiện tại vào counter
   	machine->WriteRegister(PrevPCReg, counter); //lưu giá trị counter vào thanh ghi lệnh trước đó
    	counter = machine->ReadRegister(NextPCReg); //lưu giá trị thanh ghi tiếp theo vào counter
    	machine->WriteRegister(PCReg, counter); //lưu giá trị counter vào thanh ghi lệnh hiện hiện tại
   	machine->WriteRegister(NextPCReg, counter + 4); //lưu giá trị counter+4 vào thanh ghi lệnh kế tiếp

}
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------


void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch (which)
    {
    case NoException:
        return;
    case SyscallException:
        switch (type)
        {
        case SC_Halt:
            DEBUG('a', "\n Shutdown, initiated by user program.");
            interrupt->Halt();
            break;

        case SC_PrintString:
        {
            int Address;
            char *buffer;
            Address = machine->ReadRegister(4); //đọc địa chỉ bắt đầu của vùng nhớ chứa chuỗi sẽ in
            buffer = User2System(Address, MAX_BUFFER_LENGTH); //copy chuỗi từ vùng nhớ của userspace sang kernel
            int length;
            length = 0;
            while (buffer[length] != '\0') //đếm độ dài thật sự của chuỗi muốn in
            {
                length++;
            }
            gSynchConsole->Write(buffer, length + 1);  //in chuỗi ra console

	    delete[] buffer; //giải phóng vùng nhớ
            IncreasePC(); //tăng program counter
            
            break;
        }

	    case SC_ReadString:
        {
            int Address;
	        int length;
	        char* buffer;

	        Address = machine->ReadRegister(4); //lấy địa chỉ vùng nhớ ở userspace sẽ lưu chuỗi ở console
	        length = machine->ReadRegister(5);  //lấy độ dài tối đa của chuỗi

	        buffer = User2System(Address, length); //copy chuỗi từ userspace sang kernel	    
	        gSynchConsole->Read(buffer, length); //đọc chuỗi từ userspace với độ dài tối đa
	        int truelength = 0;
	        while (buffer[truelength] != '\0') //đếm độ dài thật sự của chuỗi
                {
                    truelength++;
                }
	        System2User(Address, truelength, buffer); //copy chuỗi từ vùng nhớ kernel sang userspace
	        delete[] buffer;  //giải phóng vùng nhớ
	        IncreasePC();  //tăng program counter

	        break;
        }

	case SC_PrintChar:
	{
		char ch = (char)machine->ReadRegister(4);
		gSynchConsole->Write(&ch, 1);
		IncreasePC();
		break;
	}

	case SC_ReadChar:
	{
		int maxBytes = 255;
		char* buffer = new char[255];
		int numBytes = gSynchConsole->Read(buffer, maxBytes);

		if(numBytes > 1) {
			printf("Chi duoc nhap duy nhat 1 ky tu!\n");
			DEBUG('a', "\nERROR: Chi duoc nhap duy nhat 1 ky tu!");
			machine->WriteRegister(2, 0);
		}
		else if(numBytes == 0) {
			printf("Ky tu rong!\n");
			DEBUG('a', "\nERROR: Ky tu rong!");
			machine->WriteRegister(2, 0);
		}
		else {
			char c = buffer[0];
			machine->WriteRegister(2, c);
		}
		delete buffer;
		IncreasePC();
		
		break;		
	}
        
        case SC_PrintInt:
        {
            //// đọc từ thanh ghi
            //int number = machine->ReadRegister(4); //đọc địa chỉ bắt đầu của vùng nhớ chứa chuỗi sẽ in
            //char* buffer = NULL;
            //
            //
            //// length
            //int length = 0;
            //int temp = number;
            //do {
            //    temp /= 10;
            //    length++;
            //} while (temp > 0);


            //// fix negative
            //int end = 0;
            //if (number > 0)
            //{
            //    number *= -1;
            //    buffer = new char[length + 1];
            //}
            //else
            //{
            //    buffer = new char[length + 1 + 1];
            //    buffer[0] = '-';
            //    end = 1;
            //}


            //// chuyển số thành chuỗi
            //temp = number;
            //int i = length - 1;
            //do {
            //    int digit = temp % 10;
            //    temp /= 10;
            //    buffer[i] = (digit + '0');
            //    --i;
            //} while (i >= end && temp > 0);
            //buffer[length] = '\0';


            int number = machine->ReadRegister(4);;
            int length = 0;

            if (0 == number) {
                length = 1;
                gSynchConsole->Write("0", 1);
            }
            bool isNegative = (number < 0 ? true : false);
            if (isNegative) number *= -1;

            //get length of number
            length = 0;
            int temp_number = number;
            do {
                temp_number /= 10;
                ++length;
            } while (temp_number > 0);

            length += (isNegative ? 1 : 0);

            //+1 for null-terminated, +1 for sure
            char* buffer = new char[length + 1 + 1];

            //cannot allocated because of something => error
            if (!buffer) break;

            //add '-' if number is negative
            if (isNegative)
            {
                buffer[0] = '-';
            }

            //reuse temp_number as copy of number
            temp_number = number;

            //iterator
            int i = length - 1;

            //Get absolute value
            do {
                int digit = temp_number % 10;
                temp_number /= 10;
                buffer[i] = (digit + '0');
                --i;
            } while (i >= 0 && temp_number > 0);

            //add null-terminated
            buffer[length] = NULL;


            // in chuỗi 
            gSynchConsole->Write(buffer, length + 1);  

            delete[] buffer; //giải phóng vùng nhớ
            IncreasePC(); //tăng program counter
            break;
        }

        default:
            printf("Unexpected user mode exception %d %d\n", which, type);
            ASSERT(FALSE);
            break;
        }
    }
}
