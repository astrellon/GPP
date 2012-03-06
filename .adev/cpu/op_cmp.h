#pragma once
#include "op_codes.h"

#define DiffCheck(diff) \
	cpu->equal = diff == 0;	\
	cpu->lessThan = diff < 0;	\
	cpu->greaterThan = diff > 0;

void cmp_c_r(opArgs) {
	int c = data[ip++];
	int r = data[ip++];
#ifdef DEBUG
	printf("Comparing c %d and r %d (%d)\n", c, r, cpu->regs[r]);
#endif
	int diff = c - cpu->regs[r];
	DiffCheck(diff);
}

void cmp_r_c(opArgs) {
	int r = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("Comparing r %d (%d) and c %d\n", r, cpu->regs[r], c);
#endif
	int diff = cpu->regs[r] - c;
	DiffCheck(diff);
}

void cmp_r_r(opArgs) {
	int r1 = data[ip++];
	int r2 = data[ip++];
#ifdef DEBUG
	printf("Comparing r %d and r %d\n", cpu->regs[r1], cpu->regs[r2]);
#endif
	int diff = cpu->regs[r1] - cpu->regs[r2];
	DiffCheck(diff);
}

class OpCmpCR : public OpClass {
public:
	OpCmpCR() : OpClass("cmp") {
		func = cmp_c_r;
		paramTypes.push_back(P_CONST);
		paramTypes.push_back(P_REG);
	}
	~OpCmpCR(){}
};

class OpCmpRC : public OpClass {
public:
	OpCmpRC() : OpClass("cmp") {
		func = cmp_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpCmpRC(){}
};

class OpCmpRR : public OpClass {
public:
	OpCmpRR() : OpClass("cmp") {
		func = cmp_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpCmpRR(){}
};

void not_op(opArgs) {
	cpu->equal = !cpu->equal;
	cpu->lessThan = !cpu->lessThan;
	cpu->greaterThan = !cpu->greaterThan;
}

class OpNot : public OpClass {
public:
	OpNot() : OpClass("not") {
		func = not_op;
	}
	~OpNot(){}
};
