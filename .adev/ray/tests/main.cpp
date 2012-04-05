#include <iostream>

#include "test_asserts.h"
#include "test_transform.h"

int main() {
	runSuite( AssertTests );
	runSuite( TransformTests );
	
	std::cout << "\nDone!\n";
	return 0;
}
