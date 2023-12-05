#include "syscall.h"
#include "copyright.h" 

int main()
{
    //Giới thiệu nhóm:
    PrintString("Thong tin nhom: \n");
    PrintString("1. 21120535 - Nguyen Dinh Hoang Quan\n");
    PrintString("2. 21120544 - Le Minh Sang\n");
    PrintString("3. 21120551 - Dam Tu Tam\n");
    PrintString("4. 21120582 - Dinh Hoang Trung\n");
    PrintString("5. 21120588 - Nguyen Phuoc Anh Tuan\n");
    

    //Sơ lược về chương trình: 
    PrintString("- So luoc ve chuong trinh: \n");
    PrintString("\t+ Ben canh 2 chuong trinh ascii, help và sort\n");
    PrintString("\t+ Co cac chuong trinh kiem thu rieng cua tung System call nhu:\n");
    PrintString("\t\t* PrintChar\n");
    PrintString("\t\t* ReadChar\n");
    PrintString("\t\t* PrintString\n");
    PrintString("\t\t* ReadString\n");
    PrintString("\t\t* PrintInt\n");
    PrintString("\t\t* ReadInt\n");
    PrintString("\t\t* ...\n");


    //Hướng dẫn sử dụng chương trình:
    PrintString("- Chuong trinh nay se in ra bang ma ASCII\n");
    PrintString("\t+ Tu ky tu co ma ASCII 32 den 126 [khoang gia tri co the hien thi tren man hinh console]\n");
    PrintString("\t+ Chuong trinh se dung cac System call PrintChar, PrintInt, PrintString de in ra bang ma ASCII\n");
    PrintString("\t+ In ra gia tri decimal - ki tu.\n");

    //Mô tả chương trình sort:
    PrintString("- Chuong trinh sort se sap xep cac so nguyen theo thu tu tang dan/giam dan\n");
    PrintString("\t+ Nhap vao so luong phan tu cua mang tối đa 100 phan tu\n");
    PrintString("\t+ Nhap vao cac phan tu cua mang\n");
    PrintString("\t+ Lua chon sap xep tang dan/giam dan.\n");
    PrintString("\t+ Chuong trinh se dung thuat toan Bubble sort de sap xep.\n");
    PrintString("\t+ Chuong trinh se in ra mang da sap xep\n");

    Halt();
    return 0;
}