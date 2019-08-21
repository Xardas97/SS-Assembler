#include "relocationSymbol.h"

string toHex(unsigned long long value, int byteSize);

RelocationSymbol::RelocationSymbol(int offset, string type, int section, string symbol){
	this->offset = offset;
	this->type = type;
	this->section = section;
	this->symbol = symbol;
}

string RelocationSymbol::toString() {
	string tabs = "\t\t";
	string ret = "";

	ret += ("    " + toHex(offset, 5) + tabs + type + tabs);
	if (section >= 0) ret += to_string(section);
	else ret += "und";
	return ret += (tabs + symbol);
}
