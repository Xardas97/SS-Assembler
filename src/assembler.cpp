#include"assembler.h"
#include"lineParser.h"
#include"instructionParser.h"
#include<iostream>

Assembler::Assembler(string input, string output){
	this->input.open(input);
	this->output.open(output);

	symbolTable = SymbolTable();
	sectionTable = SectionTable();
	equTable = list<EquSymbol>();
	InstructionParser::initMaps();
}

string toHex(unsigned long long , int);
string format(string);
string swapBytes(string);

void Assembler::secondPass(vector<string> input) {
	list<RelocationSymbol> relocationTable;

	string directive;
	string outputFormating;

	int locCounter = 0;
	int currentSection = -1;
	string currentSectionName = "";
	string bytes = "";
	tempOutput = "";

	for (string line : input) {
		try {
			LineParser lineParser = LineParser(line, equTable);

			switch (lineParser.getType()) {
			case LineParser::Section:
				if (currentSection >= 0) {
					tempOutput += format(bytes);
					bytes = "";
					printRelocation(currentSectionName, relocationTable);
				}
				currentSection++;
				currentSectionName = lineParser.getSection();
				locCounter = 0;
				relocationTable = list<RelocationSymbol>();
				outputFormating = "======" + currentSectionName + "===================================================================================================";
				tempOutput += ("\n" + outputFormating.substr(0, 105) + "\n");
				break;
			case LineParser::Directive:
				directive = lineParser.getDirective();
				if (directive == "byte") {
					list<int> values = lineParser.getValues();
					locCounter += values.size();
					for(int value: values) bytes += toHex(value, 1);
				}
				if (directive == "word") {
					list<int> values = lineParser.getValues();
					locCounter += (2 * values.size());
					for (int value : values) bytes += swapBytes(toHex(value, 2));
				}
				else if (directive == "skip") for (int i = 0; i < lineParser.getValue(); i++, locCounter++) bytes += "00";
				else if (directive == "align") while (locCounter%lineParser.getValue() != 0) { locCounter++; bytes += "00"; }
				else if (directive == "global") symbolTable.setGlobal(lineParser.getSymbol());
				break;
			case LineParser::Instruction: {
				InstructionParser instrParser = InstructionParser(lineParser.getInstruction(), locCounter);
				locCounter += instrParser.getInstrSize();
				bytes += (toHex(instrParser.createInstrCode(symbolTable, equTable, relocationTable), 
								instrParser.getInstrSize()));
				break;
			}
			}
		}
		catch (SyntaxError e) {
			cout << "SYNTAX ERROR in line: " << line << endl; 
			if (e.getMessage() != "") cout << e.getMessage() << endl;
			exit(-2);
		}
	}
	if (currentSection >= 0) {
		tempOutput += format(bytes);
		printRelocation(currentSectionName, relocationTable);
	}
	tempOutput += "\n\n\n";

	symbolTable.print(output);
	sectionTable.print(output);
	output << tempOutput;
}

void Assembler::printRelocation(string sectionName, list<RelocationSymbol> relocationTable){
	string outputFormating = "======.rel " + sectionName + "===================================================================================================";
	tempOutput += ("\n" + outputFormating.substr(0, 105) + "\n");
	tempOutput += "      Offset            Type          Section         Symbol\n";
	tempOutput += "---------------------------------------------------------------------------------------------------------\n";
	for (RelocationSymbol rs : relocationTable) tempOutput += (rs.toString() + "\n");
	tempOutput += "=========================================================================================================";
}

vector<string> Assembler::firstPass() {
	string line;
	string label;
	string directive;

	vector<string> output = vector<string>();

	int locCounter = 0;
	int currentSection = -1;

	getline(input, line);

	while (line.find(".end") == string::npos) {
		if(int(line[line.size() - 1]) == 13) line = line.substr(0, line.size() - 1);

		if (line.empty()) { getline(input, line); continue; }

		try {
			LineParser lineParser = LineParser(line, equTable);

			//add label
			label = lineParser.getLabel();
			if (!label.empty()) {
				if (currentSection < 0) throw SyntaxError("Not in a Section");
				symbolTable.add(label, currentSection, locCounter);
			}
			if (label.length() > 23) throw SyntaxError("Label too long, max characters: 23");

			switch (lineParser.getType()) {
			case LineParser::Section:
				currentSection++;
				locCounter = 0;
				sectionTable.add(lineParser.getSection(), currentSection, lineParser.getFlags());
				break;
			case LineParser::Directive:
				directive = lineParser.getDirective();
				if (directive == "byte") locCounter += lineParser.getValues().size();
				else if (directive == "word") locCounter += (2 * lineParser.getValues().size());
				else if (directive == "skip") locCounter += lineParser.getValue();
				else if (directive == "align") while (locCounter%lineParser.getValue() != 0) locCounter++;
				else if (directive == "equ") {
					symbolTable.add(lineParser.getSymbol(), currentSection, locCounter);
					equTable.push_back(EquSymbol(lineParser.getSymbol(), lineParser.getValue()));
				}
				else if (directive == "extern") symbolTable.add(lineParser.getSymbol(), -1, 0, true);
				break;
			case LineParser::Instruction: {
				if (currentSection < 0) throw SyntaxError();
				InstructionParser instrParser = InstructionParser(lineParser.getInstruction(), locCounter);
				locCounter += instrParser.getInstrSize();
				break;
			}
			}
		}
		catch (SyntaxError e) {
			cout << "SYNTAX ERROR in line: " << line << endl; 
			if (e.getMessage() != "") cout << e.getMessage() << endl;
			exit(-2);
		}

		output.push_back(line);
		getline(input, line);

	}

	return output;
}


