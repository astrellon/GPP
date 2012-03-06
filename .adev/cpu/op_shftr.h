#pragma once
#include "op_codes.h"

void shftr_r_c(opArgs) {
	int r = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("applying right shift of c %d to r %d\n", c, r);
#endif
	cpu->regs[r] >>= c;
}
void shftr_r_r(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG
	printf("applying right shift of r %d to r %d\n", rs, rd);
#endif
	cpu->regs[rd] >>= cpu->regs[rs];
}

void shftr_r_r_r(opArgs) {
	int rd = data[ip++];
	int rs1 = data[ip++];
	int rs2 = data[ip++];
#ifdef DEBUG
	printf("applying right shift of r %d to r %d in r %d\n", rs1, rs2, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs1] >> cpu->regs[rs2];
}

void shftr_r_c_r(opArgs) {
	int rd = data[ip++];
	int c = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG	
	printf("applying right shift of c %d to r %d in r %d\n", c, rs, rd);
#endif
	cpu->regs[rd] = c >> cpu->regs[rs];
}

void shftr_r_r_c(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("applying right shift of r %d to c %d in r %d\n", rs, c, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs] >> c;
}


class OpShftrRC : public OpClass {
public:
	OpShftrRC() : OpClass("shftr") {
		func = shftr_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpShftrRC(){}
};

class OpShftrRR : public OpClass {
public:
	OpShftrRR() : OpClass("shftr") {
		func = shftr_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpShftrRR(){}
};

class OpShftrRRR : public OpClass {
public:
	OpShftrRRR() : OpClass("shftr") {
		func = shftr_r_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpShftrRRR(){}
};

class OpShftrRCR : public OpClass {
public:
	OpShftrRCR() : OpClass("shftr") {
		func = shftr_r_c_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
		paramTypes.push_back(P_REG);
	}
	~OpShftrRCR(){}
};

class OpShftrRRC : public OpClass {
public:
	OpShftrRRC() : OpClass("shftr") {
		func = shftr_r_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpShftrRRC(){}
};
