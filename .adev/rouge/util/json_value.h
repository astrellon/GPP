#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <ostream>

#include "tokeniser.h"

namespace am {
namespace util {

enum JsonType {
	JV_NULL,
	JV_UNDEF,
	JV_INT,
	JV_FLOAT,
	JV_BOOL,
	JV_STR,
	JV_OBJ,
	JV_ARR,
	JV_CMT
};

using namespace std;

class JsonValue;

typedef map<string, JsonValue> JsonObject;
typedef vector<JsonValue> JsonArray;

typedef union {
	int i;
	float f;
	bool b;
	string *s;
	JsonObject *o;
	JsonArray *a;
} JsonUnion;

/*#define ALT_REFS(type, refMap, value, retain)	\
	map<const type *, int>::iterator iter;	\
	iter = refMap.find(value);	\
	if (iter != refMap.end()) {	\
		int count = iter->second + (retain ? 1 : -1);	\
		if (count <= 0) {	\
			const type *ptr = iter->first;	\
			refMap.erase(iter);	\
			delete ptr;	\
		}	\
		else {	\
			iter->second = count;	\
		}	\
	}	\
	else if (retain) {	\
		refMap[value] = 1;	\
	}*/

class JsonValue {
public:
	JsonValue();
	JsonValue(JsonType type);
	JsonValue(int val);
	JsonValue(float val);
	JsonValue(bool val);
	JsonValue(const char *val);
	JsonValue(const string &val);
	JsonValue(const JsonArray &val);
	JsonValue(const JsonObject &val);
	JsonValue(JsonArray *val);
	JsonValue(JsonObject *val);
	JsonValue(const JsonValue &val);
	~JsonValue();
	
	JsonType getType() const;
	const char *getTypeName() const;
	int getInt() const;
	float getFloat() const;
	bool getBool() const;
	string *getStr() const;
	const char *getCStr() const;
	JsonObject *getObj() const;
	JsonArray *getArr() const;

	bool castFloat(float &value) const;
	
	JsonValue operator[](int i) const;
	JsonValue operator[](const char *i) const;
	JsonValue operator[](const string &i) const;
	JsonValue &operator[](int i);
	JsonValue &operator[](const char *i);
	JsonValue &operator[](const string &i);
	
	JsonValue at(int i) const;
	JsonValue at(const char *i) const;
	JsonValue at(const string &i) const;

	JsonValue &at(int i);
	JsonValue &at(const char *i);
	JsonValue &at(const string &i);

	int length() const;

	bool has(const char *i) const;
	bool has(const string &i) const;

	bool has(const char *i, JsonType type) const;
	bool has(const string &i, JsonType type) const;

	JsonValue &operator=(const JsonValue &rhs);
	bool operator==(const JsonValue &rhs) const;
	bool operator!=(const JsonValue &rhs) const;
	
	static JsonValue JsonUndef;

	void display(ostream &stream = cout);

	static JsonValue import(const char *str);
	static JsonValue import_from_file(const char *filename);
	
protected:
	JsonType mType;
	JsonUnion mContent;

	void checkRefs(bool retain);

	void altStrRef(const string *str, bool retain);
	void altObjRef(const JsonObject *obj, bool retain);
	void altArrRef(const JsonArray *arr, bool retain);

	void displayValue(ostream &stream, JsonValue &value, int depth);

	template<class type>
	void ALT_REFS(map<const type *, int> &refMap, const type *value, bool retain) {
		map<const type *, int>::iterator iter;
		iter = refMap.find(value);
		if (iter != refMap.end()) {
			int count = iter->second + (retain ? 1 : -1);
			if (count <= 0) {
				const type *ptr = iter->first;
				refMap.erase(iter);
				delete ptr;
			}
			else {
				iter->second = count;
			}
		}
		else if (retain) {
			refMap[value] = 1;
		}
	}

	static JsonValue import_object(Tokeniser &tokeniser, const char *startToken = NULL);

private:
	static map<const string *, int> sStrRefs;
	static map<const JsonObject *, int> sObjRefs;
	static map<const JsonArray *, int> sArrRefs;
	static const char *sTypeNames[];

	void displayDepth(ostream &stream, int depth);

	static char getChar(const char *token);
	static const char *nextToken(Tokeniser &tokeniser, bool skipComments = true);
};

}
}

#ifdef _ANDROID
#	include "json_value.cpp"
#endif
