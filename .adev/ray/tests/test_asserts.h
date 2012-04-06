#pragma once

#include "test_suite.h"

class AssertTests : public TestSuite {
public:
	virtual void runCases() {
		runCase(test1);
	}
	virtual const char *getSuiteName() const {
		return "Asserts";
	}
private:
	bool test1() {
		assert(true);
		//assert(false);
		return true;
	}
};
