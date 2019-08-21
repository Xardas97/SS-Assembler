#pragma once

#include<string>
#include"assemblerErrors.h"
#include"equSymbol.h"
#include<list>

using namespace std;

class LineParser {
public:
	enum LineType {Empty, Section, Directive, Instruction};

	LineParser(string,list<EquSymbol>) throw(SyntaxError);

	string getLabel() const;
	string getSection() const;
	string getDirective() const;
	string getInstruction() const;
	
	list<int>& getValues();
	int getValue() const;
	string getSymbol() const;
	string getFlags() const;
	
	LineType getType() const;
private:
	string label;
	string section;
	string directive;
	string instruction;

	LineType type;

	list<int> values;
	int value;
	string symbol;
	string flags;

	static string sections[4];
	static string directives[7];

	void parse(string line, list<EquSymbol>) throw(SyntaxError);
};
