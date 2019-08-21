#include "sectionTable.h"

SectionTable::SectionTable() {
	listOfSections = list<Section>();
}

void SectionTable::add(string label, int section, string flags) {
	listOfSections.push_back(Section(label, section, flags));
}

void SectionTable::print(ostream& out) const {
	out << "==============================================SECTION TABLE==============================================\n"
		<< "   Name              Section                   Flags\n"
		<< "---------------------------------------------------------------------------------------------------------\n";
	for (Section s : listOfSections)
		out << s << "\n";
	out << "=========================================================================================================";
}
