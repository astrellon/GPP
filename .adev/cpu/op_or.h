#pragma once
#include "op_codes.h"

void or_r_c(opArgs) {
	int r = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("oring c %d and r %d\n", c, r);
#endif
	cpu->regs[r] |= c;
}
void or_r_r(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG
	printf("oring r %d and r %d\n", rs, rd);
#endif
	cpu->regs[rd] |= cpu->regs[rs];
}

void or_r_r_r(opArgs) {
	int rd = data[ip++];
	int rs1 = data[ip++];
	int rs2 = data[ip++];
#ifdef DEBUG
	printf("oring r %d and r %d in r %d\n", rs1, rs2, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs1] | cpu->regs[rs2];
}

void or_r_c_r(opArgs) {
	int rd = data[ip++];
	int c = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG	
	printf("oring c %d and r %d in r %d\n", c, rs, rd);
#endif
	cpu->regs[rd] = c | cpu->regs[rs];
}

void or_r_r_c(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("oring r %d and c %d in r %d\n", rs, c, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs] | c;
}


class OpOrRC : public OpClass {
public:
	OpOrRC() : OpClass("or") {
		func = or_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpOrRC(){}
};

class OpOrRR : public OpClass {
public:
	OpOrRR() : OpClass("or") {
		func = or_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpOrRR(){}
};

class OpOrRRR : public OpClass {
public:
	OpOrRRR() : OpClass("or") {
		func = or_r_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpOrRRR(){}
};

class OpOrRCR : public OpClass {
public:
	OpOrRCR() : OpClass("or") {
		func = or_r_c_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
		paramTypes.push_back(P_REG);
	}
	~OpOrRCR(){}
};

class OpOrRRC : public OpClass {
public:
	OpOrRRC() : OpClass("or") {
		func = or_r_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpOrRRC(){}
};
