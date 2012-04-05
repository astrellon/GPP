#pragma once

#define TEST_VECTORS
//#define IN_TEST_CASE

#include <iostream>
#include <string>

using namespace std;

bool _in_test_case = false;

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

template <class T>
bool _equals(const T &expected, const T &actual, const char *file, unsigned int line) {
	if (expected != actual) {
		dispError(expected, actual, file, line);
		return false;
	}
	return true;
}

template <class T>
bool _notEquals(const T &expected, const T &actual, const char *file, unsigned int line) {
	if (expected == actual) {
		dispNotError(expected, file, line);
		return false;
	}
	return true;
}
bool _equals(const char *expected, const char *actual, const char *file, unsigned int line) {
	if (strcmp(expected, actual) != 0) {
		dispError(expected, actual, file, line);
		return false;
	}
	return true;
}
bool _notEquals(const char *expected, const char *actual, const char *file, unsigned int line) {
	if (strcmp(expected, actual) == 0) {
		dispNotError(expected, file, line);
		return false;
	}
	return true;
}

bool _equals(const double &expected, const double &actual, double delta, const char *file, unsigned int line) {
	double diff = expected - actual;
	bool equal = diff >= -delta && diff <= delta;
	if (!equal) {
		dispError(expected, actual, delta, file, line);
		return false;
	}
	return true;
}
bool _notEquals(const double &expected, const double &actual, double delta, const char *file, unsigned int line) {
	double diff = expected - actual;
	bool equal = diff >= -delta && diff <= delta;
	if (equal) {
		dispNotError(expected, actual, delta, file, line);
		return false;
	}
	return true;
}

#ifdef TEST_VECTORS
#include "../vector.h"

template <class T>
bool _equalsVector(const Vector<T> &expected, const Vector<T> &actual, double delta, const char *file, unsigned int line) {
	if (!expected.equals(actual, delta)) {
		dispError(expected, actual, delta, file, line);
		return false;
	}
	return true;
}
template <class T>
bool _notEqualsVector(const Vector<T> &expected, const Vector<T> &actual, double delta, const char *file, unsigned int line) {
	if (expected.equals(actual, delta)) {
		dispNotError(expected, actual, delta, file, line);
		return false;
	}
	return true;
}

#endif

#ifdef IN_TEST_CASE
#	define assert(a) \
	if (!_assert(a, __FILE__, __LINE__)) { return false; }

#	define equals(x, y) \
	if (!_equals(x, y, __FILE__, __LINE__)) { return false; }
#	define equalsD(x, y, d) \
	if (!_equals(x, y, d, __FILE__, __LINE__)) { return false; }
#	define equalsV(x, y, d) \
	if (!_equalsVector(x, y, d, __FILE__, __LINE__)) { return false; }

#	define notEquals(x, y) \
	if (!_notEquals(x, y, __FILE__, __LINE__)) { return false; }
#	define notEqualsD(x, y, d) \
	if (!_notEquals(x, y, d, __FILE__, __LINE__)) { return false; }
#	define notEqualsV(x, y, d) \
	if (!_notEqualsVector(x, y, d, __FILE__, __LINE__)) { return false; }
#else
#	define assert(a) _assert(a, __FILE__, __LINE__)

#	define equals(x, y) _equals(x, y, __FILE__, __LINE__)
#	define equalsD(x, y, d) _equals(x, y, d, __FILE__, __LINE__)
#	define equalsV(x, y, d) _equalsVector(x, y, d, __FILE__, __LINE__)

#	define notEquals(x, y) _notEquals(x, y, __FILE__, __LINE__)
#	define notEqualsD(x, y, d) _notEquals(x, y, d, __FILE__, __LINE__)
#	define notEqualsV(x, y, d) _notEqualsVector(x, y, d, __FILE__, __LINE__)
#endif
