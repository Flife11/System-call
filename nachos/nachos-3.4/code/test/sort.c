#include "syscall.h"

int compare(int a, int b, int type) {  // Type 1: Ascending. Type 2: Descending
    if (type == 1) {
        if (a > b) {  // Truoc > Sau thi swap
            return 0;
        } else
            return 1;
    } else {
        if (a < b) {
            return 0;
        } else
            return 1;
    }
}

int main() {
    int a[101]; /* size of physical memory; with code, we'll run out of space!*/

    int n, type, i, j, temp;

    /* first initialize the array, in reverse sorted order */
    // Nhap so ki tu
    do {
        PrintString("Nhap so ki so n (n <= 100) ");
        n = ReadInt();
    } while (n > 100 || n <= 0);

    // Chon loai sap xep
    do {

        PrintString("Nhap loai sap xep (1: Tang dan || 2: Giam dan) ");
        type = ReadInt();

    } while (type != 1 && type != 2);

    // Input array
    for (i = 0; i < n; ++i) {
        PrintString("a[");
        PrintInt(i);
        PrintString("] = ");
        a[i] = ReadInt();
    }

   
    // Bubble sort
    for (i = 0; i < n - 1; ++i) {
        for (j = 0; j < n - i - 1; ++j) {
            if (!compare(a[j], a[j + 1], type)) {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }

    // Print result
    PrintString("Mang da sap xep\n");
    for (i = 0; i < n; i++) {
        PrintInt(a[i]);
        PrintChar(' ');
    }
    
    
    Halt();
    Exit(a[0]); 
}
