#pragma once
#include "op_codes.h"

void mov_r_c(opArgs) {
	int r = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("moving c %d to r %d (%d)\n", c, r, cpu->regs[r]);
#endif
	cpu->regs[r] = c;
}
void mov_r_r(opArgs) {
	int rd = data[ip++];
	int rs = data[ip++];
#ifdef DEBUG
	printf("moving r %d (%d) to r %d (%d)\n", rs, cpu->regs[rs], rd, cpu->regs[rd]);
#endif
	cpu->regs[rd] = cpu->regs[rs];
}

class OpMovRC : public OpClass {
public:
	OpMovRC() : OpClass("mov") {
		func = mov_r_c;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpMovRC(){}
};

class OpMovRR : public OpClass {
public:
	OpMovRR() : OpClass("mov") {
		func = mov_r_r;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_REG);
	}
	~OpMovRR(){}
};

void mov_r_mr(opArgs) {
	int r = data[ip++];
	int mr = data[ip++];
#ifdef DEBUG
	printf("moving mr %d (%d) to r %d (%d)\n", mr, cpu->regs[mr], r, cpu->regs[r]);
#endif
	cpu->regs[r] = cpu->mem[cpu->regs[mr]];
}

void mov_r_mc(opArgs) {
	int r = data[ip++];
	int mc = data[ip++];
#ifdef DEBUG
	printf("moving mc %d to r %d (%d)\n", mc, r, cpu->regs[r]);
#endif
	cpu->regs[r] = cpu->mem[mc];
}

class OpMovRMR : public OpClass {
public:
	OpMovRMR() : OpClass("mov") {
		func = mov_r_mr;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_MEM_REG);
	}
	~OpMovRMR(){}
};

class OpMovRMC : public OpClass {
public:
	OpMovRMC() : OpClass("mov") {
		func = mov_r_mc;
		paramTypes.push_back(P_REG);
		paramTypes.push_back(P_MEM_CONST);
	}
	~OpMovRMC(){}
};

void mov_mr_r(opArgs) {
	int mr = data[ip++];
	int r = data[ip++];
#ifdef DEBUG
	printf("moving r %d (%d) to mr %d (%d)\n", r, cpu->regs[r], mr, cpu->regs[mr]);
#endif
	cpu->mem[cpu->regs[mr]] = cpu->regs[r];
}

void mov_mr_c(opArgs) {
	int mr = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("moving c %d to mr %d (%d)\n", c, mr, cpu->regs[mr]);
#endif
	cpu->mem[cpu->regs[mr]] = c;
}
class OpMovMRR : public OpClass {
public:
	OpMovMRR() : OpClass("mov") {
		func = mov_mr_r;
		paramTypes.push_back(P_MEM_REG);
		paramTypes.push_back(P_REG);
	}
	~OpMovMRR(){}
};

class OpMovMRC : public OpClass {
public:
	OpMovMRC() : OpClass("mov") {
		func = mov_mr_c;
		paramTypes.push_back(P_MEM_REG);
		paramTypes.push_back(P_CONST);
	}
	~OpMovMRC(){}
};

void mov_mc_r(opArgs) {
	int mc = data[ip++];
	int r = data[ip++];
#ifdef DEBUG
	printf("moving r %d (%d) to mc %d\n", r, cpu->regs[r], mc);
#endif
	cpu->mem[mc] = cpu->regs[r];
}

void mov_mc_c(opArgs) {
	int mc = data[ip++];
	int c = data[ip++];
#ifdef DEBUG
	printf("moving c %d to mc %d\n", c, mc);
#endif
	cpu->mem[mc] = c;
}

class OpMovMCR : public OpClass {
public:
	OpMovMCR() : OpClass("mov") {
		func = mov_mc_r;
		paramTypes.push_back(P_MEM_CONST);
		paramTypes.push_back(P_REG);
	}
	~OpMovMCR(){}
};

class OpMovMCC : public OpClass {
public:
	OpMovMCC() : OpClass("mov") {
		func = mov_mc_c;
		paramTypes.push_back(P_MEM_CONST);
		paramTypes.push_back(P_CONST);
	}
	~OpMovMCC(){}
};
