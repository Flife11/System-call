#include "syscall.h"
#include "copyright.h" 
#define MAX_BUFFER_LENGTH 255

int main(){

    OpenFileID fileID;
    char buffer[] = "Kiểm thử system call Write";
    int bytesRead, bytesWritten;
    // open the file for reading
    
    fileID = Open("example.txt");
    
    if (fileID == -1) {
        // fail if file couldn't be opened
        Write("Failed to open file.\n", 22, ConsoleOutput);
        return -1;
    }
    
    
    // write data to file
    bytesWritten = Write(buffer, bytesRead, fileID);
    if (bytesWritten == -1) {
        // fail if unable to write to file
        Write("Failed to write to file.\n", 26, ConsoleOutput);
        return -1;
    }
    

    Close(fileID);

    Halt();
    return 0;
}