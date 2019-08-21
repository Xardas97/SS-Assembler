#pragma once
#include<string>
#include<vector>
#include"assemblerErrors.h"


using namespace std;

class ParseHelper {
public:
	ParseHelper(string) throw(SyntaxError);
	vector<string> getParams() const;
	string getFirstParam() const;
	string getSecondParam() const;
private:
	vector<string> params;
};