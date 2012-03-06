#pragma once
#include "op_codes.h"

class OpNop : public OpClass {
public:
	OpNop() : OpClass("nop") {
	}
	~OpNop() {}
};

void call_c(opArgs) {
	int c = data[ip++];
	cpu->pushIpStack();
	ip = c;
}

void ret_func(opArgs) {
	cpu->popIpStack();
}

void end_func(opArgs) {
	cpu->running = false;
	printf("Ending program\n");
}

void push_c(opArgs) {
	int c = data[ip++];
	cpu->pushStack(c);
}

class OpCallC : public OpClass {
public:
	OpCallC() : OpClass("call") {
		func = call_c;
		paramTypes.push_back(P_CONST);
	}
	~OpCallC() {}
};

class OpReturn : public OpClass {
public:
	OpReturn() : OpClass("return") {
		func = ret_func;
	}
	~OpReturn() {}
};

class OpEnd : public OpClass {
public:
	OpEnd() : OpClass("end") {
		func = end_func;
	}
	~OpEnd() {}
};

void push_r(opArgs) {
	int r = data[ip++];
	cpu->pushStack(cpu->regs[r]);
}
void pop_r(opArgs) {
	int r = data[ip++];
	cpu->popStack(r);
}

class OpPushC : public OpClass {
public:
	OpPushC() : OpClass("push") {
		func = push_c;
		paramTypes.push_back(P_CONST);
	}
	~OpPushC() {}
};

class OpPushR : public OpClass {
public:
	OpPushR() : OpClass("push") {
		func = push_r;
		paramTypes.push_back(P_REG);
	}
	~OpPushR() {}
};

class OpPopR : public OpClass {
public:
	OpPopR() : OpClass("pop") {
		func = pop_r;
		paramTypes.push_back(P_REG);
	}
	~OpPopR() {}
};

void mem_r(opArgs) {
	int r = data[ip++];
	cpu->regs[r] = cpu->getAvailableMemory();
}

class OpMemR : public OpClass {
public:
	OpMemR() : OpClass("mem") {
		func = mem_r;
		paramTypes.push_back(P_REG);
	}
	~OpMemR() {}
};

void inc_r(opArgs) {
	int r = data[ip++];
	cpu->regs[r] = cpu->regs[r] + 1;
}

void dec_r(opArgs) {
	int r = data[ip++];
	cpu->regs[r] = cpu->regs[r] - 1;
}

class OpIncR : public OpClass {
public:
	OpIncR() : OpClass("inc") {
		func = inc_r;
		paramTypes.push_back(P_REG);
	}
	~OpIncR() {}
};

class OpDecR : public OpClass {
public:
	OpDecR() : OpClass("dec") {
		func = dec_r;
		paramTypes.push_back(P_REG);
	}
	~OpDecR() {}
};
