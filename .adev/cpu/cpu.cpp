CPU::CPU(int registerSize, int stackSize, int memSize) {
	numReg = registerSize;
	this->memSize = memSize;
	this->stackSize = stackSize;
	
	running = false;
	fatalError = false;
	
	regs = new int[numReg];
	mem = new int[memSize];
	stack = new int[stackSize];
	stackPos = 0;
	strMemPtr = memSize;
	
	ip = 0;
	ipStackPos = 0;
	ipStack = new int[stackSize];
	
	ops = new opFunc[OpClass::getCounter()];
	addOps();
	
	conditions.push_back(1);
	
	vector<string> startParams;
	startParams.push_back("jmp");
	startParams.push_back("main");
	parseLine(startParams);
	
}

int CPU::openfile(const string &filename) {
	FILE *input = fopen(filename.c_str(), "r");
	if (input == NULL) {
		printf("Unable to open input file '%s'\n", filename.c_str());
		return -1;
	}
	
	vector<int> totalData;
	printf("Opened file!\n\n");
	
	char line[128];
	int lineNum = 0;
	while (fgets(line, sizeof (line), input) != NULL) {
		lineNum++;
		size_t lineLen = strlen(line);
		
#ifdef DEBUG
		printf("Line %d, %s\n", lineLen, line);
#endif
		if (lineLen > 1) {
			char end = line[lineLen - 1];
			if (end < 32) {
				line[lineLen - 1] = '\0';
			}
#ifdef DEBUG
			printf("%d: %s\n", lineNum, line);
#endif
			
			if (lineLen > 2) {
				if (line[0] == '/' && line[1] == '/') {
					continue;
				}
			}
			
			vector<string> lineTokened;
			
			Tokeniser t(line);
			const char *token = t.nextToken();
			bool first = true;
			bool preproc = false;
			while(token != NULL) { 
				if (first && token[0] == '#') {
					preproc = true;
					token = t.nextToken();
					first = false;
					continue;
				}
				if (!preproc) {
					const char *defineCheck = getDefine(token);
					if (defineCheck != NULL) {
#ifdef DEBUG
						printf("Replacing >%s< with >%s<\n", token, defineCheck);
#endif
						token = defineCheck;
					}
				}
				string tokenStr = string(token);
				
				lineTokened.push_back(string(token));
				token = t.nextToken();
				first = false;
			}
			
			if (preproc) {
				parsePreproc(lineTokened);
			} else if (conditionOk()) {
				parseLine(lineTokened);
			}
		}
	}
	fclose(input);
}

bool CPU::conditionOk(int offset) const {
	for (int i = conditions.size() - 1 - offset; i >= 0; --i) {
#ifdef DEBUG_2
		printf("Checking cond %d, %d\n", i, conditions[i]);
#endif
		if (conditions[i] <= 0) {
			return false;
		}
	}
	return true;
}

const char *CPU::getDefine(const char *name) {
	string strName = string(name);
#ifdef DEBUG_2
	printf("Looking for define %s: ", strName.c_str());
#endif
	map<string, string>::iterator it = defined.find(strName);
	if (it == defined.end()) {
#ifdef DEBUG_2
		printf("not found\n");
#endif
		return NULL;
	}
#ifdef DEBUG_2
	printf("found >%s<\n", it->second.c_str());
#endif
	return it->second.c_str();
}

void CPU::parsePreproc(const vector<string> &params) {
	bool cond = conditionOk();
#ifdef DEBUG
	printf("Preproc for >%s< cond: %d\n", params[0].c_str(), cond);
#endif
	if (params[0].compare("define") == 0 && cond) {
		string name = params[1];
		string value = "";
		if (params.size() > 2) { 
			value = params[2];
			for (int i = 3; i < params.size(); i++) {
				value += ' ';
				value += params[i];
			}
		}
		defined[name] = value;
#ifdef DEBUG
		printf("Defining symbol %s as >%s<\n", name.c_str(), value.c_str());
#endif
	} else if (params[0].compare("include") == 0 && cond) {
		string inputFile = params[1];
		if (inputFile[0] == '"') {
			inputFile = params[1].substr(1);
		}
#ifdef DEBUG
		printf("Attempting to include '%s'\n", inputFile.c_str());
#endif
		openfile(inputFile);
	} else if (params[0].compare("ifndef") == 0) {
		if (!cond) {
#ifdef DEBUG
			printf("ifndef not cond.\n");
#endif
			conditions.push_back(0);
			return;
		}
		const char *defined = getDefine(params[1].c_str());
#ifdef DEBUG
		printf("ifndef for >%s< is %d\n", params[1].c_str(), defined == NULL);
#endif
		if (defined != NULL) {
			conditions.push_back(0);
		} else {
			conditions.push_back(1);
		}
	} else if (params[0].compare("ifdef") == 0) {
		if (!cond) {
#ifdef DEBUG
			printf("ifdef not cond.\n");
#endif
			conditions.push_back(0);
			return;
		}
		const char *defined = getDefine(params[1].c_str());
#ifdef DEBUG
		printf("ifdef for >%s< is %d\n", params[1].c_str(), defined == NULL);
#endif
		if (defined == NULL) {
			conditions.push_back(0);
		} else {
			conditions.push_back(1);
		}
	} else if (params[0].compare("endif") == 0) {
		conditions.pop_back();
	} else if (params[0].compare("else") == 0) {
		cond = conditionOk(1);
		if (!cond) {
			return;
		}
		int top = conditions.back();
		conditions.pop_back();
		conditions.push_back(!(bool)top);
	}
}

int CPU::addToMem(const char *str) {
	string memStr = string(str);
	map<string, int>::iterator it = strMemPoses.find(memStr);
	if (it != strMemPoses.end()) {
#ifdef DEBUG_4
		printf("String already in mem >%s< at %d\n", str, it->second);
#endif
		return it->second;
	}
	//int len = (int)strlen(str);
	int len = memStr.size();
	int pos = strMemPtr - len - 1;
	if (pos < 0) {
		printf("Unable to fit '%s' into memory!\n", str);
		return -1;
	}
	strMemPtr = pos;
#ifdef DEBUG
	printf("Adding >%s< to %d, %d\n", str, pos, len);
#endif
	for(int i = 0; i < len; ++i) {
		mem[pos + i] = str[i];
	}
	mem[pos + len] = '\0';
	strMemPoses[memStr] = pos;
	return pos;
}

void CPU::parseLine(const vector<string> &params) {
	const string &opName = params[0];
	if (opName.length() == 0) {
		return;
	}
	char opChar = opName[0];
	if (opChar == ':') {
		string labelName = opName.substr(1);
#ifdef DEBUG
		printf("Label %s at %d\n", labelName.c_str(), datas.size());
		labelLookup[datas.size()] = labelName;
#endif
		labelLocs[labelName] = datas.size();
		return;
	}
	OpMap &registered = OpClass::getRegistered();
	OpMap::iterator opList = registered.find(opName);
	if (opList == registered.end()) {
		printf("Unknown op '%s'\n", opName.c_str());
		return;
	}
	
	vector<OpClass*>::iterator it;
	vector<int> inputParamTypes;
	vector<int> finalParamTypes;
	for(int i = 1; i < params.size(); i++) {
		int paramType = opCheckParam(params[i]);
		if (paramType == P_LABEL ||
			paramType == P_CHAR ||
			paramType == P_STRING) {
			finalParamTypes.push_back(P_CONST);
		} else {
			finalParamTypes.push_back(paramType);
		}
		inputParamTypes.push_back(paramType);
	}
	
	bool matches = true;
	
	for(it = opList->second.begin(); it != opList->second.end(); ++it) {
		OpClass *op = *it;
		if (op->paramTypes.size() != params.size() - 1) {
			continue;
		}
		
		matches = true;
		for(int i = 0; i < params.size() - 1; i++) {
			if (finalParamTypes[i] != op->paramTypes[i]) {
				matches = false;
				break;
			}
		}
		if (matches) {
			datas.push_back(op->opCode);
			int num = (int)inputParamTypes.size();
			for(int i = 0; i < num; i++) {
				string pstr = params[i + 1];
				const char *param = pstr.c_str();
				
				int type = inputParamTypes[i];
				if (type == P_REG) {
					datas.push_back(atoi(param + 1));
				} else if (type == P_LABEL) {
					labelUseLocs[datas.size()] = params[i + 1];
#ifdef DEBUG
					printf("Use of label >%s< at %d\n", params[i + 1].c_str(), datas.size());
#endif
					datas.push_back(0);
				} else if (type == P_CHAR) {
					char ch = param[1];
					datas.push_back((int)ch);
				} else if (type == P_STRING) {
					int pos = addToMem(param + 1);
					if (pos < 0) {
						datas.push_back(0);
					} else {
						datas.push_back(pos);
					}
				} else if (type == P_MEM_CONST) {
					datas.push_back(atoi(param + 1));
				} else if (type == P_MEM_REG) {
					datas.push_back(atoi(param + 2));
				} else {
					datas.push_back(atoi(param));
				}
			}
			break;
		}
	}
	if (!matches) {
		printf("No matches found for %s: ", params[0].c_str());
		for (int i = 1; i < params.size(); i++) {
			printf("%s ", params[i].c_str());
		}
		printf("\n");
	}
}

void CPU::postParse() {
	map<int, string>::iterator it;
	for(it = labelUseLocs.begin(); it != labelUseLocs.end(); ++it) {
		int loc = labelLocs[it->second];
		datas[it->first] = loc;
	}
	labelUseLocs.clear();
	labelLocs.clear();
}

void CPU::addOps() {
	OpMap& map = OpClass::getRegistered();
	OpMap::iterator it;
	for(it = map.begin(); it != map.end(); ++it) {
		vector<OpClass*>::iterator ito;
		for(ito = it->second.begin(); ito != it->second.end(); ++ito) {
			ops[(*ito)->opCode] = (*ito)->func;
		}
	}
}

void CPU::pushIpStack() {
	ipStack[ipStackPos] = ip;
	ipStackPos++;
	if (ipStackPos >= stackSize) {
		printf("IP Stack overflow at %d\n", ip);
		fatalError = true;
		running = false;
		return;
	}
}

void CPU::popIpStack() {
	ipStackPos--;
	if (ipStackPos < 0) {
		printf("IP Stack underflow at %d\n", ip);
		fatalError = true;
		running = false;
		return;
	}
	ip = ipStack[ipStackPos];
}

void CPU::pushStack(int val) {
	stack[stackPos] = val;
	stackPos++;
	if (stackPos >= stackSize) {
		printf("Stack overflow at %d\n", ip);
		fatalError = true;
		running = false;
	}
}

void CPU::popStack(int reg) {
	stackPos--;
	if (stackPos < 0) {
		printf("IP Stack underflow at %d\n", ip);
		fatalError = true;
		running = false;
		return;
	}
	regs[reg] = stack[stackPos];
}
