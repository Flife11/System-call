//stable.h  
#ifndef STABLE_H  
#define STABLE_H  
#include "synch.h"  
#include "bitmap.h"  
#define MAX_SEMAPHORE 10  


class Sem {
private:
	char name[50];
	Semaphore* sem; // Tạo Semaphore để quản lý
public:
	// khởi tạo đối tượng Sem. Gán giá trị ban đầu là null
	// nhớ khởi tạo bm sử dụng
	Sem(char* na, int i) {
		strcpy(this->name, na);
		sem = new Semaphore(name, i);
	}
	~Sem() {
		delete sem;
	}
	void wait() {			// thực hiện thao tác chờ
		sem->P();
	}
	void signal() {			// thực hiện thao tác giải phóng Semaphore
		sem->V();
	}
	char* GetName() {
		return name;		// Trả về tên của Semaphore
	}
};
class STable {   
private:  
	BitMap* bm;								// quản lý slot trống
	Sem* semTab[MAX_SEMAPHORE];				// quản lý tối đa 10 đối tượng Sem
public:
	// khởi tạo size đối tượng Sem để quản lý 10 Semaphore. Gán giá trị ban đầu là null
	// nhớ khởi tạo bm để sử dụng
	STable();    
	~STable();    
	int Create(char* name);		// Kiểm tra Semaphore “name” chưa tồn tại thì tạo Semaphore mới. Ngược lại, báo lỗi
	int Wait(char* name);		// Nếu tồn tại Semaphore “name” thì gọi this->P() để thực thi. Ngược lại, báo lỗi
	int Signal(char* name);		// Nếu tồn tại Semaphore “name” thì gọi this->V() để thực thi. Ngược lại, báo lỗi
	int FindFreeSlot(int id);	// Tìm slot trống
};
#endif 
