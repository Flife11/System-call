#include "syscall.h"

int main() {

	int i;
	for (i = 0; i < 1000; ++i) {
		Down("print_c");	
		PrintChar('a');
		Up("print_f");
	}
	Exit(0);
}
