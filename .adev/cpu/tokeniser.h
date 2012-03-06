#pragma once

#include <cstring>

class Tokeniser {
public:
	Tokeniser(const char *str) {
		int len = (int)strlen(str);
		ctokMem = ctok = new char[len + 1];
		strcpy(ctok, str);
		first = true;
#ifdef DEBUG
		printf("Tokenising >%s<\n", str);
#endif
	}
	~Tokeniser() {
		delete ctokMem;
	}

	const char *nextToken() {
		if (ctok == NULL || 
			ctok[0] == '\0' ||
			ctok[0] == '\n') {
			return NULL;
		}
		int len = (int)strlen(ctok);
		char ch = *ctok;
		while (ch == ' ' || ch == '\t') {
			ch = *(++ctok);
		}
		bool inDQ = false;
		bool inSQ = false;
		int j = 0;
		bool escaped = false;
		int i = 0;
		for(; i < len; i++) {
			char ch = ctok[i];
			if (!inDQ && ch == '"') {
				inDQ = true;
				tokBuff[j++] = '"';
				continue;
			} else if (!inSQ && ch == '\'') {
				inSQ = true;
				tokBuff[j++] = '\'';
				continue;
			} else if ((!inDQ && !inSQ) && (ch == ' ' || ch == '\t')) {
				break;
			}
			if ((inDQ || inSQ) && ch == '\\') {
				escaped = true;
				continue;
			}
		
			if (escaped) {
				if (ch == 'n') {
					ch = '\n';
				} else if (ch == 't') {
					ch = '\t';
				} else if (ch == '0') {
					ch = '\0';
				}
			}
		
			if (inDQ && !escaped && ch == '"') {
				i++;
				break;
			} else if (inSQ && !escaped && ch == '\'') {
				i++;
				break;
			}
			escaped = false;
			if (ch == '\0') {
				break;
			}
			tokBuff[j++] = ch;
#ifdef DEBUG_2
			printf("Adding char to buff %d, %c from %d\n", ch, ch, i + (ctok - ctokMem)); 
#endif
			if (ch == '#' && first) {
				first = false;
				i++;
				break;
			}
			first = false;
		 }
		 tokBuff[j] = '\0';
#ifdef DEBUG
		 printf("Found token len %d >%s<\n", j, tokBuff);
#endif
		 ctok += i;
		 return tokBuff; 
	}
	
private:
	char *ctokMem;
	char *ctok;
	char tokBuff[128];
	bool first;
};
