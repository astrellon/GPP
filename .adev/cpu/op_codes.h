#pragma once

#define P_CONST	0
#define P_REG		1
#define P_LABEL	2
#define P_MEM_CONST	3
#define P_MEM_REG	4
#define P_CHAR	5
#define P_STRING 6

#include <string>
#include <vector>
#include <map>

using std::string;

int opCheckParam(const string &paramStr) {
	const char *param = paramStr.c_str();
	char c1 = param[0];
	if (c1 == '\'') {
		return P_CHAR;
	}
	if (c1 == '"') {
		return P_STRING;
	}
	bool mem = false;
	if (c1 == '@') {
		mem = true;
		param++;
		c1 = param[0];
	}
	if ((c1 >= '0' && c1 <= '9') || c1 == '-') {
		if (mem) {
			return P_MEM_CONST;
		}
		return P_CONST;
	}
	char c2 = '\0';
	if (strlen(param) > 1) {
		c2 = param[1];
	}
	if ((c1 == 'r' || c1 == 'R') &&
		(c2 >= '0' && c2 <= '9')) {
		if (mem) {
			return P_MEM_REG;
		}
		return P_REG;
	}
	return P_LABEL;
}

using namespace std;

class CPU;
#define opArgs const vector<int>& data, int &ip, CPU *cpu
typedef void (*opFunc) (opArgs);
class OpClass;
typedef vector<OpClass*> OpList;
typedef map<string, vector<OpClass*> > OpMap;

void nop_func(opArgs) {
#ifdef DEBUG
	printf("NOP\n");
#endif
}

class OpClass {
public:
	OpClass(string op) {
		func = nop_func;
		opName = op;
		opCode = nextCounter();
		registerOp(this);
	}
	~OpClass() {}
	
	vector<int> paramTypes;
	opFunc func;
	string opName;
	int opCode;
	
	static int getCounter() {
		return opCounter;
	}
	static OpMap &getRegistered() {
		return registeredOps;
	}
private:
	static int opCounter;
	
	static int nextCounter() {
		return opCounter++;
	}
	static void registerOp(OpClass *op) {
		registeredOps[op->opName].push_back(op);
	}
	
	static void deregisterOp(OpClass *op) {
		OpMap::iterator opNameIt = registeredOps.find(op->opName);
		if (opNameIt != registeredOps.end()) {
			
		}
	}
	
	static OpMap registeredOps;
};

int OpClass::opCounter = 0;
OpMap OpClass::registeredOps;
