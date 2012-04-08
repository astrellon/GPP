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
		runCase( testOrbit );
	}
	virtual const char *getSuiteName() const {
		return "Transform";
	}
private:
	double d;
	bool testDefaults() {
		Transform t;
		equals(Vector4f(0, 1, 0), t.getUp());
		equals(Vector4f(1, 0, 0), t.getRight());
		equals(Vector4f(0, 0, 1), t.getForward());
		
		equals(Vector4f(0, 0, 0), t.getPosition());
		equals(Vector4f(0, 0, 1), t.getTarget());
		return true;
	}
	
	bool testOrbit() {
		Transform t;
		const double hpi = M_PI * 0.5;
		t.orbit(hpi, 0);
		equals(Vector4f(0, 1, 0), t.getUp());
		equals(Vector4f(0, 0, -1), t.getRight());
		equals(Vector4f(-1, 0, 0), t.getForward());
		
		equals(Vector4f(1, 0, 1), t.getPosition());
		equals(Vector4f(0, 0, 1), t.getTarget());
		
		return true;
	}
};
