#pragma once

//#define TEST_VECTORS
#define IN_TEST_CASE

#include <iostream>
#include <string>

using namespace std;

inline void dispErrorLine(const char *file, int line) {
	cout << "\nError in file " << file << '[' << line << "]\n";
}

bool _assert(bool a, const char *file, int line) {
	if (!a) {
		dispErrorLine(file, line);
		cout << "- Assert failed!\n\n";
		return false;
	}
	return true;
}

template <class T>
void dispError(const T &expected, const T &actual, const char *file, unsigned int line) {
	dispErrorLine(file, line);
	cout << "- Expected: " << expected << "\n- Actual  : " << actual << "\n\n";
}

template <class T>
void dispNotError(const T &expected, const char *file, unsigned int line) {
	dispErrorLine(file, line);
	cout << "- Did not expect: " << expected << "\n\n";
}

template <class T>
void dispError(const T &expected, const T &actual, double delta, const char *file, unsigned int line) {
	dispErrorLine(file, line);
	cout << "- Expected: " << expected << "\n- Actual  : " << actual << "\n- Delta   : " << delta << "\n\n";
}

template <class T>
void dispNotError(const T &expected, const T &actual, double delta, const char *file, unsigned int line) {
	dispErrorLine(file, line);
	cout << "- Did not expect: " << expected << "\n- Actual: " << actual << "\n- Delta   : " << delta << "\n\n";
}

#define _simple_compare(e, a, r, f, l)	\
	if (r) {	\
	if (e == a) { dispNotError(e, f, l); return false; }	return true;	\
	} else {	\
	if (e != a) { dispError(e, a, f, l);	return false; }	return true; }

bool _equals(const char *file, unsigned int line, const int &expected, const int &actual, bool notCompare) {
	_simple_compare(expected, actual, notCompare, file, line);
}
bool _equals(const char *file, unsigned int line, const unsigned int &expected, const unsigned int &actual, bool notCompare) {
	_simple_compare(expected, actual, notCompare, file, line);
}
bool _equals(const char *file, unsigned int line, const double &expected, const double &actual, bool notCompare) {
	_simple_compare(expected, actual, notCompare, file, line);
}

bool _equals(const char *file, unsigned int line, const char *expected, const char *actual, bool notCompare) {
	if (notCompare) {
		if (strcmp(expected, actual) == 0) {
			dispError(expected, actual, file, line);
			return false;
		}
		return true;
	}
	if (strcmp(expected, actual) != 0) {
		dispError(expected, actual, file, line);
		return false;
	}
	return true;
}

bool _equals(const char *file, unsigned int line, const double &expected, const double &actual, bool notCompare, double delta=0.00001) {
	double diff = expected - actual;
	bool equal = diff >= -delta && diff <= delta;
	if (notCompare) {
		if (equal) {
			dispError(expected, actual, delta, file, line);
			return false;
		}
		return true;
	}
	if (!equal) {
		dispError(expected, actual, delta, file, line);
		return false;
	}
	return true;
}

#ifdef TEST_VECTORS
#include "../vector.h"

template <class T>
bool _equals(const char *file, unsigned int line, const Vector<T> &expected, const Vector<T> &actual, bool notCompare, double delta=0.00001) {
	if (notCompare) {
		if (expected.equals(actual, delta)) {
			dispError(expected, actual, delta, file, line);
			return false;
		}
		return true;
	}
	else {
		if (!expected.equals(actual, delta)) {
			dispError(expected, actual, delta, file, line);
			return false;
		}
		return true;
	}
}
#endif

#ifdef IN_TEST_CASE
#	define assert(a) \
	if (!_assert(a, __FILE__, __LINE__)) { return false; }

#	define equals(expected, actual) \
	if (!_equals(__FILE__, __LINE__, expected, actual, false)) { return false; }
#	define equalsDelta(expected, actual, delta) \
	if (!_equals(__FILE__, __LINE__, expected, actual, false, delta)) { return false; }

#	define notEquals(expected, actual) \
	if (!_equals(__FILE__, __LINE__, expected, actual, true)) { return false; }
#	define notEqualsDelta(expected, actual, delta) \
	if (!_equals(__FILE__, __LINE__, expected, actual, true, delta)) { return false; }
#else
#	define assert(a) _assert(a, __FILE__, __LINE__)

#	define equals(expected, actual) _equals(__FILE__, __LINE__, expected, actual, false)
#	define equalsDelta(expected, actual, delta) _equals(__FILE__, __LINE__, expected, actual, false, delta)

#	define notEquals(expected, actual) _equals(__FILE__, __LINE__, expected, actual, true)
#	define notEqualsDelta(expected, actual, delta) _equals(__FILE__, __LINE__, expected, actual, true, delta)
#endif
