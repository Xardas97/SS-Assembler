#pragma once

#include<string>
#include<iostream>

using namespace std;

class Symbol {
public:
	Symbol(string label, int section, int offset, bool global) {
		this->label = label;
		this->section = section;
		this->offset = offset;
		this->global = global;
	}

	string label;
	int section;
	int offset;
	bool global;

	friend ostream& operator<<(ostream&, Symbol&);
};
