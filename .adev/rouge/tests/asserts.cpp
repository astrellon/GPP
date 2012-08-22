#include "asserts.h"

namespace am {
namespace tests {

	void Asserts::dispErrorLine(const char *file, int line) {
		stringstream ss;
		ss << "Error in file " << file << '[' << line << "]";
		am_log("ERR", ss);
	}

	bool Asserts::_assert(bool a, const char *file, int line) {
		if (!a) {
			dispErrorLine(file, line);
			am_log("ERR", "- Assert failed!\n");
			return false;
		}
		return true;
	}

	bool Asserts::_equals(const char *file, unsigned int line, const int &expected, const int &actual, bool notCompare) {
		_simple_compare(expected, actual, notCompare, file, line);
	}
	bool Asserts::_equals(const char *file, unsigned int line, const unsigned int &expected, const unsigned int &actual, bool notCompare) {
		_simple_compare(expected, actual, notCompare, file, line);
	}
	bool Asserts::_equals(const char *file, unsigned int line, const double &expected, const double &actual, bool notCompare) {
		_simple_compare(expected, actual, notCompare, file, line);
	}

	bool Asserts::_equals(const char *file, unsigned int line, const char *expected, const char *actual, bool notCompare) {
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

	bool Asserts::_equals(const char *file, unsigned int line, const double &expected, const double &actual, bool notCompare, double delta) {
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

}
}