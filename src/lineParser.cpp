#include "lineParser.h"
#include "parseHelper.h"

string LineParser::sections[] = { "text", "data", "bss", "section" };
string LineParser::directives[] = { "byte", "word", "align", "skip", "extern", "global", "equ" };

int toInt(string str);

LineParser::LineParser(string line, list<EquSymbol> equTable) throw(SyntaxError) {
	label = section = directive = instruction = "";
	type = Empty;

	parse(line, equTable);
}

void LineParser::parse(string line, list<EquSymbol> equTable) throw(SyntaxError) {
	int pos;

	//if comment
	if ((pos = line.find(";")) != string::npos)
		line = line.substr(0, pos);

	//if label
	if ((pos = line.find(":")) != string::npos) {
		//cut out spaces infront of the label
		int start = line.find_first_not_of(" \t");
		label = line.substr(start, pos-start);

		//label can't be empty and can't contain spaces
		if (label.empty() || label.find_first_of(" \t")!=string::npos) throw SyntaxError("Empty Label Name");

		//cut spaces after the label
		line = line.substr(pos + 1, line.length() - (pos + 1));
		if ((pos = line.find_first_not_of(" \t")) != string::npos)
			line = line.substr(pos, line.length() - pos);
		else line = "";
	}

	if ((pos = line.find_first_not_of(" \t")) == string::npos) {
		type = Empty;
		return;
	}
	line = line.substr(pos, line.length() - pos);

	//if directive or section
	if ((pos = line.rfind(".")) != string::npos) {
		if (pos != 0) throw SyntaxError("Unexpected '.' character");
		line = line.substr(1, line.length() - 1);

		//test if section
		for (int i = 0; i < 4; i++)
			if (line.rfind(sections[i], 0) == 0) {
				section = sections[i];
				if (section == "section") {
					line = line.substr(8, line.length() - 8); //cut out .section
					ParseHelper parseHelper = ParseHelper(line);
					section = parseHelper.getFirstParam();
					if (section.length() > 23) throw SyntaxError("Symbol name too long, max characters: 23");
					flags = parseHelper.getSecondParam();
				}
				if (section == "text") { flags = "rx"; }
				else if (section == "data") { flags = "rw"; }
				else if (section == "bss") { flags = "r"; }
				type = Section;
				return;
			}
		//test if directive
		for (int i = 0; i < 7; i++)
			if (line.rfind(directives[i], 0) == 0) {
				directive = directives[i];
				line = line.substr(directive.length(), line.length() - directive.length());
				type = Directive;
				if (directive == "word" || directive == "byte") {
					ParseHelper parseHelper = ParseHelper(line);
					for (string param : parseHelper.getParams()) {
						int value = toInt(param);
						for (EquSymbol sym : equTable)
							if (sym.label == param) {
								value = sym.value;
								break;
							}
						values.push_back(value);
					}
				}
				if (directive == "skip" || directive == "align") {
					if (line.find_first_not_of(" \t") == string::npos) throw SyntaxError("Missing Parameter");
					else value = toInt(line);
				}
				else if (directive == "equ") {
					ParseHelper parseHelper = ParseHelper(line);
					if (parseHelper.getSecondParam() == "") throw SyntaxError("Missing Parameter");
					symbol = parseHelper.getFirstParam();
					if (symbol.length() > 23) throw SyntaxError("Symbol name too long, max characters: 23");
					value = toInt(parseHelper.getSecondParam());
				}
				else if (directive == "extern" || directive == "global") {
					ParseHelper parseHelper = ParseHelper(line);
					symbol = parseHelper.getFirstParam();
				}
				return;
			}
		//if none but had a '.', throw syntax error
		throw SyntaxError("Unexpected '.' character");
	}

	//instruction
	type = Instruction;
	instruction = line;
}

string LineParser::getLabel() const {
	return label;
}

string LineParser::getSection() const {
	return section;
}

string LineParser::getDirective() const {
	return directive;
}


string LineParser::getInstruction() const {
	return instruction;
}

list<int>& LineParser::getValues() {
	return values;
}

int LineParser::getValue() const {
	return value;
}

string LineParser::getSymbol() const {
	return symbol;
}

string LineParser::getFlags() const {
	return flags;
}

LineParser::LineType LineParser::getType() const {
	return type;
}
