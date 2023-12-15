#include "ptable.h"
#include "system.h"
#include "openfile.h"



PTable::PTable(int size)
{
    if (size < 0)
        return;

    psize = size;
    bm = new BitMap(size);
    bmsem = new Semaphore("bmsem",1);

    for(int i=0; i<MAX_PROCESS ;i++){
		pcb[i] = 0;
    }

	bm->Mark(0);

	pcb[0] = new PCB(0);
	pcb[0]->SetFileName("./test/scheduler");
	pcb[0]->parentID = -1;
}

PTable::~PTable()
{
    if( bm != 0 )
	delete bm;
    
    for(int i=0;i<psize;i++){
		if(pcb[i] != 0)
			delete pcb[i];
    }
		
	if( bmsem != 0)
		delete bmsem;
}

int PTable::ExecUpdate(char* name)
{
	//Gọi mutex->P();tránh nạp 2 tiến trình cùng 1 lúc.
	bmsem->P();
	
	// Đảm bảo thread không thực hiện chính nó
	if(strcmp(name, currentThread->getName()) == 0 ||  strcmp(name,"./test/scheduler") == 0)
	{ 
		printf("\tCouldnt execute program, thread cant run itself.\n");		
		bmsem->V();
		return -1;
	}
	// Kiểm tra tồn tại chương trình tên name, nếu không có thì báo lỗi, trả về -1
	if(name == NULL)
	{
		printf("\tCouldnt execute program, name is NULL.\n");
		bmsem->V();
		return -1;
	}
	// Tìm slot trống trong bảng Ptable.
	int freeSlotIndex = this->GetFreeSlot();

    // Kiểm có slot trống hay không
	if(freeSlotIndex < 0)
	{
		printf("\tNo free slot found.\n");
		bmsem->V();
		return -1;
	}

	// Nếu có slot trống thì tạo một PCB mới với processID chính là index của slot này
	pcb[freeSlotIndex] = new PCB(freeSlotIndex);
	pcb[freeSlotIndex]->SetFileName(name);

	// parrentID là processID của currentThread
    	pcb[freeSlotIndex]->parentID = currentThread->processID;

	
	//Gọi exec
	int pid = pcb[freeSlotIndex]->Exec(name, freeSlotIndex);

	bmsem->V();
	
	return pid;
}

int PTable::JoinUpdate(int id)
{
	// Kiểm tra xem tiến trình gọi join có phải là cha của tiến trình processID là id hay không
	if(id < 0)
	{
		printf("\t Error JoinUpdate: id = %d", id);
		return -1;
	}
	// Kiểm tra cha của process join có phải process của current thread không
	if(currentThread->processID != pcb[id]->parentID)
	{
		printf("\tError JoinUpdate: Couldnt join process to its parent.\n");
		return -1;
	}

    // Tăng numwait và gọi JoinWait() để chờ tiến trình con thực hiện.
	// Sau khi tiến trình con thực hiện xong, tiến trình đã được giải phóng
	pcb[pcb[id]->parentID]->IncNumWait();
	
	
	pcb[id]->JoinWait();

	int ec = pcb[id]->GetExitCode();
	// Tiến trình con thoát
	pcb[id]->ExitRelease();

	return ec;
}
int PTable::ExitUpdate(int exitcode)
{              
    // Nếu tiến trình gọi là main process thì gọi Halt().
	int id = currentThread->processID;
	if(id == 0)
	{
		// giải phóng bộ nhớ
		currentThread->FreeSpace();		
		interrupt->Halt();
		return 0;
	}
    
        if(IsExist(id) == false)
	{
		printf("\tError ExitUpdate: This id: %d does not exist", id);
		return -1;
	}
	
	// Ngược lại gọi SetExitCode để đặt exitcode cho tiến trình gọi.
	pcb[id]->SetExitCode(exitcode);
	pcb[pcb[id]->parentID]->DecNumWait();
    
    // Giải phóng tiến trình cha đang đợi nó (nếu có)
	pcb[id]->JoinRelease();
    // ExitWait() để xin tiến trình cha cho phép thoát
	pcb[id]->ExitWait();
	
	Remove(id);
	return exitcode;
}

// Find free slot in order to save the new process infom
int PTable::GetFreeSlot()
{
	return bm->Find();
}

// Check if Process ID is Exist
bool PTable::IsExist(int pid)
{
	return bm->Test(pid);
}

// Remove proccess ID out of table
// When it ends
void PTable::Remove(int pid)
{
	bm->Clear(pid);
	if(pcb[pid] != 0)
		delete pcb[pid];
}

char* PTable::GetFileName(int id)
{
	return (pcb[id]->GetFileName());
}

