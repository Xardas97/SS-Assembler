#include "symbol.h"

string toHex(unsigned long long value, int byteSize);

ostream & operator<<(ostream& out, Symbol& s){
	string tabs = "\t\t\t", tabsName = "";
	for (int i = 0; i <= (23 - s.label.length()); i+=8)
		tabsName += "\t";
	

	out << s.label << tabsName;
	if (s.section == -1) out << "und";
	else out << s.section;
	out << tabs << toHex(s.offset, 5) << tabs;
	if (s.global) out << "global";
	else out << "local";
	return out;
}
