#pragma once
#include<list>
#include<iostream>
#include"symbol.h"
#include "assemblerErrors.h"

class SymbolTable {
public:
	SymbolTable();
	void add(string label, int section, int offset);
	void add(string label, int section, int offset, bool global);
	void setGlobal(string symbol) throw(SyntaxError);
	void print(ostream&) const;
	Symbol* find(string label);
private:
	list<Symbol> listOfSymbols;
};
