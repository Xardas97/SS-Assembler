#pragma once
#include<list>
#include<iostream>
#include"section.h"
#include "assemblerErrors.h"

class SectionTable {
public:
	SectionTable();
	void add(string label, int section, string flags);
	void print(ostream&) const;
private:
	int freeNumber;
	list<Section> listOfSections;
};
