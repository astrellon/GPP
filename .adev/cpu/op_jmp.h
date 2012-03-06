#pragma once
#include "op_codes.h"

void jmp_c(opArgs) {
	int c = data[ip];
	ip = c;
#ifdef DEBUG
	printf("jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
}
void jmp_r(opArgs) {
	int r = data[ip];
	ip = cpu->regs[r];
#ifdef DEBUG
	printf("jumping r %s\n", cpu->labelLookup[ip].c_str());
#endif
}

class OpJmpC : public OpClass {
public:
	OpJmpC() : OpClass("jmp") {
		func = jmp_c;
		paramTypes.push_back(P_CONST);
	}
	~OpJmpC(){}
};

class OpJmpR : public OpClass {
public:
	OpJmpR() : OpClass("jmp") {
		func = jmp_r;
		paramTypes.push_back(P_REG);
	}
	~OpJmpR(){}
};

void jeq_c(opArgs) {
	int c = data[ip++];
	if (cpu->equal) {
		ip = c;
#ifdef DEBUG
		printf("equal jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}

void jeq_r(opArgs) {
	int r = data[ip++];
	if (cpu->equal) {
		ip = cpu->regs[r];
#ifdef DEBUG
		printf("equal jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}

class OpJeqR : public OpClass {
public:
	OpJeqR() : OpClass("jeq") {
		func = jeq_r;
		paramTypes.push_back(P_REG);
	}
	~OpJeqR(){}
};

class OpJeqC : public OpClass {
public:
	OpJeqC() : OpClass("jeq") {
		func = jeq_c;
		paramTypes.push_back(P_CONST);
	}
	~OpJeqC(){}
};


void jneq_c(opArgs) {
	int c = data[ip++];
	if (!cpu->equal) {
		ip = c;
#ifdef DEBUG
		printf("not equal jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}

void jneq_r(opArgs) {
	int r = data[ip++];
	if (!cpu->equal) {
		ip = cpu->regs[r];
#ifdef DEBUG
		printf("not equal jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}

class OpJneqR : public OpClass {
public:
	OpJneqR() : OpClass("jneq") {
		func = jneq_r;
		paramTypes.push_back(P_REG);
	}
	~OpJneqR(){}
};

class OpJneqC : public OpClass {
public:
	OpJneqC() : OpClass("jneq") {
		func = jneq_c;
		paramTypes.push_back(P_CONST);
	}
	~OpJneqC(){}
};

void jlt_r(opArgs) {
	int r = data[ip++];
	if (cpu->lessThan) {
		ip = cpu->regs[r];
#ifdef DEBUG
		printf("less than jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}
void jlt_c(opArgs) {
	int c = data[ip++];
	if (cpu->lessThan) {
		ip = c;
#ifdef DEBUG
		printf("less than jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}

class OpJltR : public OpClass {
public:
	OpJltR() : OpClass("jlt") {
		func = jlt_r;
		paramTypes.push_back(P_REG);
	}
	~OpJltR(){}
};

class OpJltC : public OpClass {
public:
	OpJltC() : OpClass("jlt") {
		func = jlt_c;
		paramTypes.push_back(P_CONST);
	}
	~OpJltC(){}
};

void jlte_r(opArgs) {
	int r = data[ip++];
	if (cpu->lessThan || cpu->equal) {
		ip = cpu->regs[r];
#ifdef DEBUG
		printf("less than equal jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}
void jlte_c(opArgs) {
	int c = data[ip++];
	if (cpu->lessThan || cpu->equal) {
		ip = c;
#ifdef DEBUG
		printf("less than equal jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}

class OpJlteR : public OpClass {
public:
	OpJlteR() : OpClass("jlte") {
		func = jlte_r;
		paramTypes.push_back(P_REG);
	}
	~OpJlteR(){}
};

class OpJlteC : public OpClass {
public:
	OpJlteC() : OpClass("jlte") {
		func = jlte_c;
		paramTypes.push_back(P_CONST);
	}
	~OpJlteC(){}
};

void jgt_r(opArgs) {
	int r = data[ip++];
	if (cpu->greaterThan) {
		ip = cpu->regs[r];
#ifdef DEBUG
		printf("greater than jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}
void jgt_c(opArgs) {
	int c = data[ip++];
	if (cpu->greaterThan) {
		ip = c;
#ifdef DEBUG
		printf("greater than jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}

class OpJgtR : public OpClass {
public:
	OpJgtR() : OpClass("jgt") {
		func = jgt_r;
		paramTypes.push_back(P_REG);
	}
	~OpJgtR(){}
};

class OpJgtC : public OpClass {
public:
	OpJgtC() : OpClass("jgt") {
		func = jgt_c;
		paramTypes.push_back(P_CONST);
	}
	~OpJgtC(){}
};

void jgte_r(opArgs) {
	int r = data[ip++];
	if (cpu->greaterThan || cpu->equal) {
		ip = cpu->regs[r];
#ifdef DEBUG
		printf("greater than equal jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}
void jgte_c(opArgs) {
	int c = data[ip++];
	if (cpu->greaterThan || cpu->equal) {
		ip = c;
#ifdef DEBUG
		printf("greater than equal jumping %s\n", cpu->labelLookup[ip].c_str());
#endif
	}
}

class OpJgteR : public OpClass {
public:
	OpJgteR() : OpClass("jgte") {
		func = jgte_r;
		paramTypes.push_back(P_REG);
	}
	~OpJgteR(){}
};

class OpJgteC : public OpClass {
public:
	OpJgteC() : OpClass("jgte") {
		func = jgte_c;
		paramTypes.push_back(P_CONST);
	}
	~OpJgteC(){}
};
