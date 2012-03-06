#include <stdio.h>
#include <stdlib.h>

#include <string>

//#define DEBUG
//#define DEBUG_2
//#define DEBUG_3
//#define DEBUG_4

using std::string;

#include "add_ops.h"
#include "cpu.h"

int main() {
	
	addAllOps();
	CPU cpu(32, 128, 1024);
	string filename = "prog1.asm";
	cpu.openfile(filename);
	
	cpu.postParse();
	removeAllOpClasses();
	cpu.execute();
	
	return 0;
}
