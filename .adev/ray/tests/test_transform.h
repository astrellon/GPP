#pragma once

#include "test_suite.h"
#include "../transform.h"
#include "../vector.h"
#include "../matrix.h"

class TransformTests : public TestSuite {
public:
	virtual void runCases() {
		d = 0.00001;
		runCase( testDefaults );
	}
	virtual const char *getSuiteName() const {
		return "Transform";
	}
private:
	double d;
	bool testDefaults() {
		Transform trans;
		equals(Vector4f(0, 1, 0), trans.getUp());
		equals(Vector4f(1, 0, 0), trans.getRight());
		equals(Vector4f(0, 0, 1), trans.getForward());
		
		equals(Vector4f(0, 0, 0), trans.getPosition());
		equals(Vector4f(0, 0, 1), trans.getTarget());
		return true;
	}
};
