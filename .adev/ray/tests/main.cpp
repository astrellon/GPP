#include <iostream>

#include "asserts.h"
#include "../vector.h"

using namespace std;

int main() {
	equals(5,5);
	equals(3,6);
	equalsD(2.505, 2.50, 0.1);
	equalsD(2.505, 2.50, 0.00001);
	
	equals(Vector4f(0, 1, 2), Vector4f(0, 1.5, 2));
	return 0;
}
