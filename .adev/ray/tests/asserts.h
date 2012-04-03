#pragma once

#define TEST_VECTORS

#include <iostream>
#include <string>

using namespace std;

template <class T>
void dispError(const T &expected, const T &actual, const char *file, unsigned int line) {
	cout << "\nError in file " << file << '[' << line << "]\n";
	cout << "- Expected: " << expected << "\n- Actual  : " << actual << "\n\n";
}

template <class T>
void dispError(const T &expected, const T &actual, double delta, const char *file, unsigned int line) {
	cout << "\nError in file " << file << '[' << line << "]\n";
	cout << "- Expected: " << expected << "\n- Actual  : " << actual << "\n- Delta   : " << delta << "\n\n";
}

template <class T>
void _equals(const T &expected, const T &actual, const char *file, unsigned int line) {
	if (expected != actual) {
		dispError(expected, actual, file, line);
	}
}

void _equals(const char *expected, const char *actual, const char *file, unsigned int line) {
	if (strcmp(expected, actual) != 0) {
		dispError(expected, actual, file, line);
	}
}

void _equals(const double &expected, const double &actual, double delta, const char *file, unsigned int line) {
	double diff = expected - actual;
	bool equal = diff >= -delta && diff <= delta;
	if (!equal) {
		dispError(expected, actual, delta, file, line);
	}
}

#ifdef TEST_VECTORS
#include "../vector.h"

template <class T>
void _equals(const Vector<T> &expected, const Vector<T> &actual) {
	const double delta = 0.00001;
	if (!expected.equals(actual, delta)) {
		dispError(expected, actual, delta);
	}
}

#endif

#define equals(x, y) _equals(x, y, __FILE__, __LINE__)
#define equalsD(x, y, d) _equals(x, y, d, __FILE__, __LINE__)
