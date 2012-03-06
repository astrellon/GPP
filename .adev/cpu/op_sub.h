#pragma once
#include "op_codes.h"

void sub_r_c(opArgs) {
	int r = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("subbing c %d from r %d\n", c, r);
#endif
	cpu->regs[r] -= c;
}
void sub_r_r(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG
	printf("subbing r %d from r %d\n", rs, rd);
#endif
	cpu->regs[rd] -= cpu->regs[rs];
}

void sub_r_r_r(opArgs) {
	int rd = data[ip++];
	int rs1 = data[ip++];
	int rs2 = data[ip++];
#ifdef DEBUG
	printf("subbing r %d from r %d in r %d\n", rs1, rs2, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs1] - cpu->regs[rs2];
}

void sub_r_c_r(opArgs) {
	int rd = data[ip++];
	int c = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG
	printf("subbing c %d from r %d in r %d\n", c, rs, rd);
#endif
	cpu->regs[rd] = c - cpu->regs[rs];
}

void sub_r_r_c(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("subbing r %d from c %d in r %d\n", rs, c, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs] - c;
}

class OpSubRC : public OpClass {
public:
	OpSubRC() : OpClass("sub") {
		func = sub_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpSubRC(){}
};

class OpSubRR : public OpClass {
public:
	OpSubRR() : OpClass("sub") {
		func = sub_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpSubRR(){}
};

class OpSubRRR : public OpClass {
public:
	OpSubRRR() : OpClass("sub") {
		func = sub_r_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpSubRRR(){}
};

class OpSubRCR : public OpClass {
public:
	OpSubRCR() : OpClass("sub") {
		func = sub_r_c_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
		paramTypes.push_back(P_REG);
	}
	~OpSubRCR(){}
};

class OpSubRRC : public OpClass {
public:
	OpSubRRC() : OpClass("sub") {
		func = sub_r_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpSubRRC(){}
};
