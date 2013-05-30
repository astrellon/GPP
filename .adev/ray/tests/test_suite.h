#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

#define runSuite(s)	\
	{	\
		s suite;	\
		suite.run();	\
	}
	
#define runCase(f)	\
	beforeCase();	\
	if (!f()) { testsFailed++; }	\
	totalTests++;	\
	afterCase();

class TestSuite {
public:
	TestSuite() {
		testsFailed = 0;
		totalTests = 0;
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
		printf("%d of %d passed ", (totalTests - testsFailed), totalTests);

		if (testsFailed > 0) {
			printf("FAIL!\n");
		}
		printf("\n");
		afterSuite();
		
	}
	virtual void runCases() = 0;
	
	virtual const char *getSuiteName() const {
		return "Unnamed";
	}

protected:
	int testsFailed;
	int totalTests;
};
