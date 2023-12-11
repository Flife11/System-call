#ifndef PTABLE_H
#define PTABLE_H

#include "bitmap.h"
#include "pcb.h"
#include "synch.h"

#define MAX_PROCESS 10

class PTable
{
private:	
	BitMap *bm;                 // đánh dấu các vị trí đã được sử dụng trong pcb
	PCB* pcb[MAX_PROCESS];
	int psize;
	Semaphore* bmsem;           // dùng để ngăn chặn trường hợp nạp 2 tiến trình cùng

public:
	// khởi tạo size đối tượng PCB để lưu size process. Gán giá trị ban đầu là null
	// nhớ khởi tạo bm và bmsem để sử dụng 
     PTable(int = 10);          
    ~PTable();                  // Huy cac doi tuong da tao
		
    int ExecUpdate(char*);      // Xử lý cho system call SC_Exit
    int ExitUpdate(int);        // Xử lý cho system call SC_Exit
    int JoinUpdate(int);        // Xử lý cho system call SC_Join
    int GetFreeSlot();          // tìm free slot để lưu thông tin cho tiến trình mới
    bool IsExist(int pid);      // kiểm tra tồn tại processID này không?    
    void Remove(int pid);       // khi tiến trình kết thúc, delete processID ra khỏi mảng quản lý nó
    char* GetFileName(int id);  // Trả về tên của tiến trình
};
#endif // PTABLE_H

