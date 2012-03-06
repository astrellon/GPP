#pragma once
#include "op_codes.h"

void xor_r_c(opArgs) {
	int r = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("xoring c %d and r %d\n", c, r);
#endif
	cpu->regs[r] ^= c;
}
void xor_r_r(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG
	printf("xoring r %d and r %d\n", rs, rd);
#endif
	cpu->regs[rd] ^= cpu->regs[rs];
}

void xor_r_r_r(opArgs) {
	int rd = data[ip++];
	int rs1 = data[ip++];
	int rs2 = data[ip++];
#ifdef DEBUG
	printf("xoring r %d and r %d in r %d\n", rs1, rs2, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs1] ^ cpu->regs[rs2];
}

void xor_r_c_r(opArgs) {
	int rd = data[ip++];
	int c = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG	
	printf("xoring c %d and r %d in r %d\n", c, rs, rd);
#endif
	cpu->regs[rd] = c ^ cpu->regs[rs];
}

void xor_r_r_c(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("xoring r %d and c %d in r %d\n", rs, c, rd);
#endif
	cpu->regs[rd] = cpu->regs[rs] ^ c;
}


class OpXorRC : public OpClass {
public:
	OpXorRC() : OpClass("xor") {
		func = xor_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpXorRC(){}
};

class OpXorRR : public OpClass {
public:
	OpXorRR() : OpClass("xor") {
		func = xor_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpXorRR(){}
};

class OpXorRRR : public OpClass {
public:
	OpXorRRR() : OpClass("xor") {
		func = xor_r_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpXorRRR(){}
};

class OpXorRCR : public OpClass {
public:
	OpXorRCR() : OpClass("xor") {
		func = xor_r_c_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
		paramTypes.push_back(P_REG);
	}
	~OpXorRCR(){}
};

class OpXorRRC : public OpClass {
public:
	OpXorRRC() : OpClass("xor") {
		func = xor_r_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpXorRRC(){}
};
