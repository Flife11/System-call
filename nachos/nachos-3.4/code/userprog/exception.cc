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

        case SC_Print:
        {
            int Address;
            char *buffer;
            Address = machine->ReadRegister(4);
            buffer = User2System(Address, MAX_BUFFER_LENGTH);
            int length;
            length = 0;
            while (buffer[length] != '\0')
            {
                length++;
            }
            synchConsole->Write(buffer, length + 1); 

	    delete[] buffer;
            machine->IncreasePC();
            
            break;
        }
        case SC_Open:
	{
	    // thông số cần có 
	    int address = machine->ReadRegister(4);
	    int fileType = machine->ReadRegister(5);
	    char* filename = User2System(address, 32);
	
	    // tìm vị trí trống trong bảng mô tả file
	    int availableIndex = -1; // error
	    for (int i = 2; i < 10; i++) // 0:stdin, 1:stdout
	        if (fileSystem->openf[i] == NULL)
	        {
	            availableIndex = i;
	            break;
	        }
	
	    // mở file
	    if (availableIndex != -1)
	    {
	        switch (fileType)
	        {
	        case 0:
	        case 1:
	            if (fileSystem->openf[availableIndex] = fileSystem->Open(filename, fileType))
	                machine->WriteRegister(2, availableIndex);
	            break;
	        case 2:
	            machine->WriteRegister(2, 0);
	            break;
	        case 3:
	            machine->WriteRegister(2, 1);
	            break;
	        default:
	            break;
	        }
	        delete[] filename;
	        break;
	    }
	    machine->WriteRegister(2, -1); // error 
	    delete[] filename;
	    machine->IncreasePC();
	    break;
	}
	
	case SC_Close:
	{
	    int id = machine->ReadRegister(4);
	    if (id >= 0 && id <= 14)
	    {
	        if (fileSystem->openf[id])
	        {
	            delete fileSystem->openf[id];
	            fileSystem->openf[id] = NULL;
	            machine->WriteRegister(2, 0);
	            break;
	        }
	    }
	    machine->WriteRegister(2, -1);
	    machine->IncreasePC();
	    break;
	}

        default:
            printf("Unexpected user mode exception %d %d\n", which, type);
            ASSERT(FALSE);
            break;
        }
    }
}
