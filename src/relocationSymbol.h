#pragma once
#include<string>

using namespace std;

class RelocationSymbol {
public:
	RelocationSymbol(int,string,int,string);
	string toString();
private:
	int offset;
	string type;
	int section;
	string symbol;
};