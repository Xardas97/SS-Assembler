#pragma once

#include<string>
#include<iostream>

using namespace std;

class Section {
public:
	Section(string label, int section, string flag) {
		this->label = label;
		this->section = section;
		this->flag = flag;
	}

	string label;
	int section;
	string flag;

	friend ostream& operator<<(ostream&, Section&);
};
