#pragma once
#include "op_codes.h"

void print_c(opArgs) {
	int c = data[ip++];
#ifdef DEBUG
	printf("print %d\n", c);
#else
	printf("%d", c);
#endif
}
void print_r(opArgs) {
	int r = data[ip++];
#ifdef DEBUG
	printf("print r%d: %d\n", r, cpu->regs[r]);
#else
	printf("%d", cpu->regs[r]);
#endif
}

void print_mr(opArgs) {
	int r = data[ip++];
#ifdef DEBUG
	printf("print mr%d: %d\n", r, cpu->mem[cpu->regs[r]]);
#else
	printf("%d", cpu->mem[cpu->regs[r]]);
#endif
}

void print_mc(opArgs) {
	int c = data[ip++];
#ifdef DEBUG
	printf("print mc%d: %d\n", c, cpu->mem[c]);
#else
	printf("%d", cpu->mem[c]);
#endif
}

class OpPrintC : public OpClass {
public:
	OpPrintC() : OpClass("print") {
		func = print_c;
		paramTypes.push_back(P_CONST);
	}
	~OpPrintC(){}
};

class OpPrintR : public OpClass {
public:
	OpPrintR() : OpClass("print") {
		func = print_r;
		paramTypes.push_back(P_REG);
	}
	~OpPrintR(){}
};

class OpPrintMR : public OpClass {
public:
	OpPrintMR() : OpClass("print") {
		func = print_mr;
		paramTypes.push_back(P_MEM_REG);
	}
	~OpPrintMR(){}
};

class OpPrintMC : public OpClass {
public:
	OpPrintMC() : OpClass("print") {
		func = print_mc;
		paramTypes.push_back(P_MEM_CONST);
	}
	~OpPrintMC(){}
};

void printc_c(opArgs) {
	int c = data[ip++];
#ifdef DEBUG
	printf("printc %d %c\n", c, c);
#else
	printf("%c", c);
#endif
}
void printc_r(opArgs) {
	int r = data[ip++];
#ifdef DEBUG
	printf("printc r%d: %c\n", r, cpu->regs[r]);
#else
	printf("%c", cpu->regs[r]);
#endif
}
void printc_mr(opArgs) {
	int r = data[ip++];
#ifdef DEBUG
	printf("print mr%d: %c\n", r, cpu->mem[cpu->regs[r]]);
#else
	printf("%c", cpu->mem[cpu->regs[r]]);
#endif
}

void printc_mc(opArgs) {
	int c = data[ip++];
#ifdef DEBUG
	printf("print mc%d: %c\n", c, cpu->mem[c]);
#else
	printf("%c", cpu->mem[c]);
#endif
}

class OpPrintC_C : public OpClass {
public:
	OpPrintC_C() : OpClass("printc") {
		func = printc_c;
		paramTypes.push_back(P_CONST);
	}
	~OpPrintC_C(){}
};

class OpPrintC_R : public OpClass {
public:
	OpPrintC_R() : OpClass("printc") {
		func = printc_r;
		paramTypes.push_back(P_REG);
	}
	~OpPrintC_R(){}
};

class OpPrintC_MR : public OpClass {
public:
	OpPrintC_MR() : OpClass("printc") {
		func = printc_mr;
		paramTypes.push_back(P_MEM_REG);
	}
	~OpPrintC_MR(){}
};

class OpPrintC_MC : public OpClass {
public:
	OpPrintC_MC() : OpClass("printc") {
		func = printc_mc;
		paramTypes.push_back(P_MEM_CONST);
	}
	~OpPrintC_MC(){}
};
