#pragma once

#include<unordered_map>
#include<string>
#include<vector>
#include<list>
#include<iostream>
#include"assemblerErrors.h"
#include"symbolTable.h"
#include"equSymbol.h"
#include"relocationSymbol.h"

using namespace std;

class InstructionParser {
public:
	InstructionParser(string,int) throw(SyntaxError);

	static void initMaps();

	struct Parameter {
		int addTypeCode = -1;
		int regCode;
		int value;
		string symbol = "";
		int offset;
		bool pcRel = false;
		bool regHigh = false;
	};

	int getInstrSize() const;
	unsigned long long createInstrCode(SymbolTable&,list<EquSymbol>,list<RelocationSymbol>&);
private:
	unsigned long long instrCode;
	int instrSize;
	bool byte;
	Parameter parseParameter(string, bool, int) throw(SyntaxError);

	int activeParams;
	vector<Parameter> params;

	static unordered_map<string, int> instructionMap;
	static unordered_map<string, int> addressingMap;
	static unordered_map<string, int> registerMap;

	static vector<string> zeroParamInstrs;
	static vector<string> oneParamInstrs;
};
