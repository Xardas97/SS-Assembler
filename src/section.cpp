#include "section.h"

ostream & operator<<(ostream& out, Section& s) {
	string tabs = "\t\t\t", tabsName = "";
	for (int i = 0; i <= (23 - s.label.length()); i+=8)
		tabsName += "\t";

	return out << s.label << tabsName << s.section << tabs << s.flag;
}
