#pragma once

#include<string>
#include<iostream>

using namespace std;

class EquSymbol {
public:
	EquSymbol(string label, int value) {
		this->label = label;
		this->value = value;
	}

	string label;
	int value;

	friend ostream& operator<<(ostream&, const EquSymbol&);
};
