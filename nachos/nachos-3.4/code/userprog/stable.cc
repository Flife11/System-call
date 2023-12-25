#include "stable.h"

STable::STable() {
    bm = new BitMap(MAX_SEMAPHORE);
    for (int i = 0; i < MAX_SEMAPHORE; i++) {
        semTab[i] = NULL;
    }
}
STable::~STable() {
    delete bm;
    for (int i = 0; i < MAX_SEMAPHORE; ++i) {
        if (semTab[i] != NULL) {
            delete semTab[i];
        }
    }
}
int STable::Create(char* name, int init) {
    // Kiểm tra xem tên semaphore đã tồn tại chưa
    for (int i = 0; i < MAX_SEMAPHORE; i++) {
        if (semTab[i] != NULL && strcmp(semTab[i]->GetName(), name) == 0) {
            // Báo lỗi vì semaphore đã tồn tại
            return -1;
        }
    }

    // Tìm slot trống
    int freeSlot = FindFreeSlot(0);

    // Nếu không tìm thấy slot trống, báo lỗi
    if (freeSlot == -1) {
        return -1;
    }

    // Tạo semaphore mới và đặt vào slot trống
    semTab[freeSlot] = new Sem(name, init);
    return 0;
}
int STable::Wait(char* name) {
    // Tìm semaphore trong semTab
    int i;
    for (i = 0; i < MAX_SEMAPHORE; ++i) {
        if (semTab[i] != NULL && strcmp(semTab[i]->GetName(), name) == 0) {
            break;
        }
    }

    // Nếu không tìm thấy semaphore, báo lỗi
    if (i == MAX_SEMAPHORE) {
        return -1;
    }

    // Gọi hàm wait() của semaphore
    semTab[i]->wait();
    return 0;
}
int STable::Signal(char* name) {
    // Tìm semaphore trong semTab
    int i;
    for (i = 0; i < MAX_SEMAPHORE; ++i) {
        if (semTab[i] != NULL && strcmp(semTab[i]->GetName(), name) == 0) {
            break;
        }
    }

    // Nếu không tìm thấy semaphore, báo lỗi
    if (i == MAX_SEMAPHORE) {
        return -1;
    }

    // Gọi hàm signal() của semaphore
    semTab[i]->signal();
    return 0;
}
int STable::FindFreeSlot(int id) {
    // Tìm slot trống từ vị trí id trở đi
    for (int i = id; i < MAX_SEMAPHORE; ++i) {
        if (bm->Test(i) == 0) {
            // Nếu slot trống, đánh dấu và trả về
            bm->Mark(i);
            return i;
        }
    }

    // Nếu không tìm thấy slot trống, thử lại từ đầu
    for (int i = 0; i < id; ++i) {
        if (bm->Test(i) == 0) {
            // Nếu slot trống, đánh dấu và trả về
            bm->Mark(i);
            return i;
        }
    }

    // Nếu không có slot trống, trả về -1
    return -1;
}
