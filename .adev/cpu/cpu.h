#pragma once

#include "op_codes.h"
#include <vector>
#include <map>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include "tokeniser.h"

using std::vector;
using std::map;
using std::stringstream;

class CPU {
public:
	CPU(int registerSize, int stackSize, int memSize);
	~CPU() {
		delete regs;
		delete mem;
		delete stack;
		delete ipStack;
		delete ops;
	}
	
	void addOps();
	
	void execute() {
		ip = 0;
		running = true;
		printf("-- Start --\n");
		int size = datas.size();
		while(running && ip < size) {
			ops[datas[ip++]](datas, ip, this);
		}
		if (fatalError) {
			printf("-- FATAL ERROR --\n");
		}
		printf("-- Completed --\n");
	}
	
	
	int openfile(const string &filename);
	void pushIpStack();
	void popIpStack();
	void pushStack(int val);
	void popStack(int reg);
	const char *getDefine(const char *name);
	void parsePreproc(const vector<string> &params);
	void parseLine(const vector<string> &params);
	void postParse();
	
	int addToMem(const char *str);
	int getAvailableMemory() const {
		return strMemPtr;
	}
	bool conditionOk(int offset = 0) const;
	
	stringstream inputBuf;
	vector<int> datas;
	map<string, int> labelLocs;
	map<int, string> labelLookup;
	map<int, string> labelUseLocs;
	map<string, string> defined;
	vector<int> conditions;
	opFunc *ops;
	int numReg;
	int memSize;
	int stackSize;
	int stackPos;
	int strMemPtr;
	map<string, int> strMemPoses;
	bool running;
	bool fatalError;
	int *regs;
	int *mem;
	int *stack;
	
	bool equal;
	bool lessThan;
	bool greaterThan;
	
	int ip;
	int ipStackPos;
	int *ipStack;
};

#include "cpu.cpp"
