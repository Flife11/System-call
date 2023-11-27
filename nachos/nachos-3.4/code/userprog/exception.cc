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
            int type = machine->ReadRegister(5); 
            char* filename = User2System(address, 32); 

            // tìm vị trí trống gần nhất trong bảng mô tả file 
            int offset = -1; // error
            for (int i = 2; i < 15; i++)
                if (fileSystem->openf[i] == NULL)
                {
                    offset = i;
                    break;
                }

            // error handling
            if (offset != -1)
            {
                switch (type)
                {
                case 0:
                case 1:
                    if (fileSystem->openf[offset] = fileSystem->Open(filename, type))
                        machine->WriteRegister(2, offset);
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
            break;
        }
        case SC_Read:
        {
            int virtAddr = machine->ReadRegister(4);  
			int charcount = machine->ReadRegister(5); 
			int id = machine->ReadRegister(6); 
			char *buf;
			if (id < 0 || id > 14)
			{
				printf("\nCan't Read to position out of domain.\n");
				machine->WriteRegister(2, -1);
				machine -> IncreasePC();
				break;;
			}
			// Kiem tra file co ton tai khong
			if (fileSystem->openf[id] == NULL)
			{
				printf("\nFile not found.\n");
				machine->WriteRegister(2, -1);
				machine -> IncreasePC();
				break;
			}
			if (fileSystem->openf[id]->type == 3)
			{
				printf("\nCan't read stdout file.\n");
				machine->WriteRegister(2, -1);
				machine -> IncreasePC();
				break;
			}
			int OldPos = fileSystem->openf[id]->getSeekPosition(); 
			buf = User2System(virtAddr, charcount);
			
			if (fileSystem->openf[id]->type == 2)
			{
				int size = synchConsole -> Read(buf, charcount);
				System2User(virtAddr, size, buf);
				machine -> WriteRegister(2, size);
				delete[] buf;
				machine -> IncreasePC();
				break;
			}
			
			if ((fileSystem->openf[id]->Read(buf, charcount)) > 0)
			{
				// So byte thuc su = NewPos - OldPos
				int NewPos = fileSystem->openf[id]->getSeekPosition();
				System2User(virtAddr, NewPos - OldPos, buf); 
				machine->WriteRegister(2, NewPos - OldPos);
			}
			else
			{
				machine->WriteRegister(2, -2);
			}
			delete[] buf;
			machine -> IncreasePC();
			break;
        }

        case SC_Write:
        {
            int virtAddr = machine->ReadRegister(4);
			int charcount = machine->ReadRegister(5);
			int openf_id = machine->ReadRegister(6);
            char* buffer;


			if (openf_id > 14 || openf_id < 0) // `out of domain` filesys + try to write to stdin 
			{
                printf("\nCan't Write to the positon out of domain.\n");
				machine->WriteRegister(2, -1);
                machine -> IncreasePC();
				break;
			}
			
			if (fileSystem->openf[openf_id] == NULL)
			{
                printf("\nFile not found.\n");
				machine->WriteRegister(2, -1);
                machine -> IncreasePC();
				break;
			}

			// read-only file	
			if (fileSystem->openf[openf_id]->type == 1 || fileSystem->openf[openf_id]->type == 2)
			{
				printf("\nCan't modify read-only file\n");
				machine->WriteRegister(2, -1);
                machine -> IncreasePC();
				break;
			}

			// write to console
                int OldPos = fileSystem->openf[openf_id]->getSeekPosition()); // Kiem tra thanh cong thi lay vi tri OldPos
                buffer = User2System(virtAddr, charcount);  // Copy chuoi tu vung nho User Space sang System Space voi bo dem buffer dai charcount
                // Xet truong hop ghi file read & write (type quy uoc la 0) thi tra ve so byte thuc su
                if (fileSystem->openf[openf_id]->type == 0)
                {
                    if ((fileSystem->openf[openf_id]->Write(buffer, charcount)) > 0)
                    {
                        // So byte thuc su = NewPos - OldPos
                        int NewPos = fileSystem->openf[openf_id]->getSeekPosition();
                        machine->WriteRegister(2, NewPos - OldPos);
                        delete[] buffer;
                        machine -> IncreasePC();
                        break;;
                    }
                }
                if (fileSystem->openf[openf_id]->type == 3) // Xet truong hop con lai ghi file stdout (type quy uoc la 3)
                {
                    int i = 0;
                    while (buffer[i] != 0 && buffer[i] != '\n') // Vong lap de write den khi gap ky tu '\n'
                    {
                        synchConsole -> Write(buffer + i, 1); // Su dung ham Write cua lop SynchConsole 
                        i++;
                    }
                    buffer[i] = '\n';
                    synchConsole -> Write(buffer + i, 1); // Write ky tu '\n'
                    machine->WriteRegister(2, i - 1); // Tra ve so byte thuc su write duoc
                    delete[] buffer;
                    machine -> IncreasePC();
                    break;;
			} 
        }

        default:
            printf("Unexpected user mode exception %d %d\n", which, type);
            ASSERT(FALSE);
            break;
        }
    }
}
