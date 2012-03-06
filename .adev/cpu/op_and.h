#pragma once
#include "op_codes.h"

void and_r_c(opArgs) {
	int r = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("anding c %d and r %d\n", c, r);
#endif
	cpu->regs[r] &= c;
}
void and_r_r(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG
	printf("anding r %d and r %d\n", rs, rd);
#endif
	cpu->regs[rd] &= cpu->regs[rs];
}

void and_r_r_r(opArgs) {
	int rd = data[ip++];
	int rs1 = data[ip++];
	int rs2 = data[ip++];
#ifdef DEBUG
	printf("anding r %d and r %d in r %d\n", rs1, rs2, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs1] & cpu->regs[rs2];
}

void and_r_c_r(opArgs) {
	int rd = data[ip++];
	int c = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG	
	printf("anding c %d and r %d in r %d\n", c, rs, rd);
#endif
	cpu->regs[rd] = c & cpu->regs[rs];
}

void and_r_r_c(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("anding r %d and c %d in r %d\n", rs, c, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs] & c;
}


class OpAndRC : public OpClass {
public:
	OpAndRC() : OpClass("and") {
		func = and_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpAndRC(){}
};

class OpAndRR : public OpClass {
public:
	OpAndRR() : OpClass("and") {
		func = and_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpAndRR(){}
};

class OpAndRRR : public OpClass {
public:
	OpAndRRR() : OpClass("and") {
		func = and_r_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpAndRRR(){}
};

class OpAndRCR : public OpClass {
public:
	OpAndRCR() : OpClass("and") {
		func = and_r_c_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
		paramTypes.push_back(P_REG);
	}
	~OpAndRCR(){}
};

class OpAndRRC : public OpClass {
public:
	OpAndRRC() : OpClass("and") {
		func = and_r_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpAndRRC(){}
};
