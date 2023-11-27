#include "syscall.h"
#include "copyright.h" 
#define MAX_BUFFER_LENGTH 255

int main(){

    OpenFileID fileID;
    char buffer[512];
    int bytesRead, bytesWritten;
    // open the file for reading
    
    fileID = Open("example.txt");
  
    if (fileID == -1) {
        // fail if file couldn't be opened
        Write("Failed to open file.\n", 22, ConsoleOutput);
        return -1;
    }

    // read data from file
    bytesRead = Read(buffer, 512, fileID);

    if (bytesRead == -1) {
        // fail if unable to read from file
        Write("Failed to read file.\n", 21, ConsoleOutput);
        return -1;
    }
    Print(buffer);
    

    Close(fileID);

    

    Halt();
    return 0;
}