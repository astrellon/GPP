#pragma once

#include "asserts.h"
#include "stdio.h"

#include <string>
using std::string;

#define runSuite(s)	\
	{	\
		s suite;	\
		suite.run();	\
	}
	
#define runCase(f)	\
	beforeCase();	\
	if (!f()) { testsFailed = true; }	\
	afterCase();

class TestSuite {
public:
	TestSuite() {
		testsFailed = false;
	}
	~TestSuite() {}
	virtual void beforeSuite() {}
	virtual void afterSuite() {}
	
	virtual void beforeCase() {}
	virtual void afterCase() {}
	
	virtual void run() {
		printf("Starting tests for '%s' ", getSuiteName());
		beforeSuite();
		runCases();
		if (testsFailed) {
			printf("failed!\n");
		}
		else {
			printf("done!\n");
		}
		afterSuite();
		
	}
	virtual void runCases() = 0;
	
	virtual const char *getSuiteName() const {
		return "Unnamed";
	}
	
protected:
	bool testsFailed;
};
