#pragma once
#include "op_codes.h"

void add_r_c(opArgs) {
	int r = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("adding c %d to r %d\n", c, r);
#endif
	cpu->regs[r] += c;
}
void add_r_r(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG
	printf("adding r %d to r %d\n", rs, rd);
#endif
	cpu->regs[rd] += cpu->regs[rs];
}

void add_r_r_r(opArgs) {
	int rd = data[ip++];
	int rs1 = data[ip++];
	int rs2 = data[ip++];
#ifdef DEBUG
	printf("adding r %d to r %d in r %d\n", rs1, rs2, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs1] + cpu->regs[rs2];
}

void add_r_c_r(opArgs) {
	int rd = data[ip++];
	int c = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG	
	printf("adding c %d to r %d in r %d\n", c, rs, rd);
#endif
	cpu->regs[rd] = c + cpu->regs[rs];
}

void add_r_r_c(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("adding r %d to c %d in r %d\n", rs, c, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs] + c;
}


class OpAddRC : public OpClass {
public:
	OpAddRC() : OpClass("add") {
		func = add_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpAddRC(){}
};

class OpAddRR : public OpClass {
public:
	OpAddRR() : OpClass("add") {
		func = add_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpAddRR(){}
};

class OpAddRRR : public OpClass {
public:
	OpAddRRR() : OpClass("add") {
		func = add_r_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpAddRRR(){}
};

class OpAddRCR : public OpClass {
public:
	OpAddRCR() : OpClass("add") {
		func = add_r_c_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
		paramTypes.push_back(P_REG);
	}
	~OpAddRCR(){}
};

class OpAddRRC : public OpClass {
public:
	OpAddRRC() : OpClass("add") {
		func = add_r_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpAddRRC(){}
};
