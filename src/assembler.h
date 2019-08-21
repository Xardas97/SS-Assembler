#pragma once
#include<string>
#include<fstream>
#include<vector>
#include"symbolTable.h"
#include"sectionTable.h"
#include"equSymbol.h"
#include"relocationSymbol.h"

using namespace std;

class Assembler {
public:
	Assembler(string input, string output);

	vector<string> firstPass();
	void secondPass(vector<string>);
private:
	ifstream input;
	ofstream output;

	string tempOutput;

	void printRelocation(string, list<RelocationSymbol>);

	SymbolTable symbolTable;
	SectionTable sectionTable;
	list<EquSymbol> equTable;
};