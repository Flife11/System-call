#include "syscall.h"

int main() {

	int i = 0;
	for (i = 0; i < 1000; ++i) {	
		Down("print_f");				
		PrintChar('b');
		Up("print_c");
	}
	Exit(0);
}
