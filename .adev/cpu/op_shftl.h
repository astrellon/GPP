#pragma once
#include "op_codes.h"

void shftl_r_c(opArgs) {
	int r = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("applying left shift of c %d to r %d\n", c, r);
#endif
	cpu->regs[r] <<= c;
}
void shftl_r_r(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG
	printf("applying left shift of r %d to r %d\n", rs, rd);
#endif
	cpu->regs[rd] <<= cpu->regs[rs];
}

void shftl_r_r_r(opArgs) {
	int rd = data[ip++];
	int rs1 = data[ip++];
	int rs2 = data[ip++];
#ifdef DEBUG
	printf("applying left shift of r %d to r %d in r %d\n", rs1, rs2, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs1] << cpu->regs[rs2];
}

void shftl_r_c_r(opArgs) {
	int rd = data[ip++];
	int c = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG	
	printf("applying left shift of c %d to r %d in r %d\n", c, rs, rd);
#endif
	cpu->regs[rd] = c << cpu->regs[rs];
}

void shftl_r_r_c(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("applying left shift of r %d to c %d in r %d\n", rs, c, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs] << c;
}


class OpShftlRC : public OpClass {
public:
	OpShftlRC() : OpClass("shftl") {
		func = shftl_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpShftlRC(){}
};

class OpShftlRR : public OpClass {
public:
	OpShftlRR() : OpClass("shftl") {
		func = shftl_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpShftlRR(){}
};

class OpShftlRRR : public OpClass {
public:
	OpShftlRRR() : OpClass("shftl") {
		func = shftl_r_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpShftlRRR(){}
};

class OpShftlRCR : public OpClass {
public:
	OpShftlRCR() : OpClass("shftl") {
		func = shftl_r_c_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
		paramTypes.push_back(P_REG);
	}
	~OpShftlRCR(){}
};

class OpShftlRRC : public OpClass {
public:
	OpShftlRRC() : OpClass("shftl") {
		func = shftl_r_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpShftlRRC(){}
};
