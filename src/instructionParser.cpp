#include"instructionParser.h"
#include"parseHelper.h"
#include<algorithm>

unordered_map<string, int> InstructionParser::instructionMap = unordered_map<string, int>();
unordered_map<string, int> InstructionParser::addressingMap = unordered_map<string, int>();
unordered_map<string, int> InstructionParser::registerMap = unordered_map<string, int>();

vector<string> InstructionParser::zeroParamInstrs = vector<string>();
vector<string> InstructionParser::oneParamInstrs = vector<string>();

int toInt(string str);
int toInt(string, int);
string toHex(unsigned long long, int);
string swapBytes(string);

unsigned long long InstructionParser::createInstrCode(SymbolTable& symbolTable, list<EquSymbol> equTable,
													list<RelocationSymbol>& relocationTable) {
	bool symbol = false;
	string relocSize = "16";
	if(byte) relocSize = "8";

	//first byte
	instrCode = instrCode << 1;
	if (!byte) instrCode += 1; 
	instrCode = instrCode << 2;

	for (int i = 0; i < activeParams; i++) {
		instrCode = (instrCode << 3); instrCode += params[i].addTypeCode;
		switch (params[i].addTypeCode) {
		case 0:
			//immediate
			instrCode = (instrCode << 5);

			if (params[i].symbol.length() > 0) {
				bool found = false;
				for (EquSymbol sym : equTable)
					if (sym.label == params[i].symbol) {
						params[i].value = sym.value;
						found = true;
						break;
					}
				if (!found) {
					symbol = true;
					Symbol* s;
					if ((s = symbolTable.find(params[i].symbol)) == nullptr)
						throw SyntaxError("Symbol not defined");
					//reloc type
					string relocType = "R_";
					if (params[i].pcRel) relocType += "PC";
					relocType += relocSize;
					relocationTable.push_back(RelocationSymbol(params[i].offset, relocType, s->section, s->label));
				}
			}
			//we have a value
			if (byte) {
				instrCode = (instrCode << 8);
				instrCode += (params[i].value %= (1 << 8));
			}
			else {
				instrCode = (instrCode << 16);
				params[i].value %= (1 << 16);
				if(!symbol) instrCode += toInt(swapBytes(toHex(params[i].value, 2)), 16);
				else instrCode += 0;
			}
			break;
		case 1:
		case 2:
			//regdir & regind
			instrCode = (instrCode << 4); instrCode += params[i].regCode;
			instrCode = (instrCode << 1); if (params[i].regHigh) instrCode += 1;
			break;
		case 3:
		case 4: 
			//reind8 & regind16
			instrCode = (instrCode << 4); instrCode += params[i].regCode;
			instrCode = (instrCode << 1);

			if (params[i].symbol.length() > 0) {
				bool found = false;
				for (EquSymbol sym : equTable)
					if (sym.label == params[i].symbol) {
						params[i].value = sym.value;
						found = true;
						break;
					}
				if (!found) {
					symbol = true;
					Symbol* s;
					if ((s = symbolTable.find(params[i].symbol)) == nullptr)
						throw SyntaxError("Symbol not defined");
					string relocType = "R_";
					if (params[i].pcRel) relocType += "PC";
					relocType += relocSize;
					relocationTable.push_back(RelocationSymbol(params[i].offset, relocType, s->section, s->label));
				}
			}
			//we have a value
			if (byte) {
				instrCode = (instrCode << 8);
				instrCode += (params[i].value %= (1 << 8));
			}
			else {
				instrCode = (instrCode << 16);
				params[i].value %= (1 << 16);
				if (!symbol) instrCode += toInt(swapBytes(toHex(params[i].value, 2)), 16);
				else instrCode += 0;
			}
			break;
		case 5: 
			//mem
			instrCode = (instrCode << 5);

			if (params[i].symbol.length() > 0) {
				symbol = true;
				Symbol* s;
				if((s = symbolTable.find(params[i].symbol)) == nullptr)
					throw SyntaxError("Symbol not defined");
				string relocType = "R_";
				if (params[i].pcRel) relocType += "PC";
				relocType += relocSize;
				relocationTable.push_back(RelocationSymbol(params[i].offset, relocType, s->section, s->label));
			}
			//we have a value
			if (byte) {
				instrCode = (instrCode << 8);
				instrCode += (params[i].value %= (1 << 8));
			}
			else {
				instrCode = (instrCode << 16);
				params[i].value %= (1 << 16);
				if(!symbol) instrCode += toInt(swapBytes(toHex(params[i].value, 2)), 16);
				else instrCode += 0;
			}
		}
	}
	return instrCode;
}

InstructionParser::InstructionParser(string instr,int offset) throw(SyntaxError) {
	string instrName;
	string firstParam = "", secondParam = "";
	params = vector<Parameter>();
	params.push_back(Parameter()); params.push_back(Parameter());
	activeParams = 0;

	instrSize = 1;
	int pos;
	byte = false;

	//cut spaces in the start of the line
	if ((pos = instr.find_first_not_of(" \t")) == string::npos) throw SyntaxError("Assembler Error");
	instr = instr.substr(pos, instr.length() - pos);

	//instruction name ends with the first space
	if ((pos = instr.find_first_of(" ,\t")) != string::npos) 
		instrName = instr.substr(0, pos);
	else instrName = instr;

	//parse and cut operand size specifier
	if (instrName[instrName.length()-1] == 'w') instrName = instrName.substr(0, instrName.length() - 1);
	else if (instrName[instrName.length()-1] == 'b' && instrName != "sub"){
		byte = true;
		instrName = instrName.substr(0, instrName.length() - 1);
	}

	try{ instrCode = instructionMap.at(instrName); }
	catch (out_of_range) { throw SyntaxError("Unknown Instruction: " + instrName); }

	if (count(zeroParamInstrs.begin(), zeroParamInstrs.end(), instrName) == 0) {
		ParseHelper parseHelper = ParseHelper(instr.substr(pos, instr.length() - pos));

		firstParam = parseHelper.getFirstParam();
		if (firstParam == "") throw SyntaxError("Missing Instruction Parameter");
		params[0] = parseParameter(firstParam, byte, offset+instrSize);
		if (params[0].addTypeCode == addressingMap["imm"]
			&& instrCode != instructionMap.at("push")
			&& instrCode != instructionMap.at("int")) throw SyntaxError("Destination Addressing Type can't be Immediate");
		if (params[0].addTypeCode == addressingMap["reg"] || params[0].addTypeCode == addressingMap["regind"]) instrSize++;
		else {
			if (byte) instrSize += 2;
			else instrSize += 3;
		}
		activeParams++;

		if (count(oneParamInstrs.begin(), oneParamInstrs.end(), instrName) == 0){
			secondParam = parseHelper.getSecondParam();
			if (secondParam == "") throw SyntaxError("Missing Instruction Parameter");
			params[1] = parseParameter(secondParam, byte, offset + instrSize);
			if (params[1].addTypeCode == addressingMap["reg"] || params[1].addTypeCode == addressingMap["regind"]) instrSize++;
			else {
				if (byte) instrSize += 2;
				else instrSize += 3;
			}
			activeParams++;
		}
	}
}

InstructionParser::Parameter InstructionParser::parseParameter(string param, bool byte, int offset) throw(SyntaxError) {
	Parameter paramStruct = Parameter();
	paramStruct.offset = offset + 1;

	if (param[0] == '&' || isdigit(param[0]) || param[0] == '-') {
		paramStruct.addTypeCode = addressingMap["imm"];
		if (param[0] == '&') paramStruct.symbol = param.substr(1, param.length() - 1);
		else paramStruct.value = toInt(param);
		return paramStruct;
	}
	if (param[0] == '$') {
		if(byte) paramStruct.addTypeCode = addressingMap["regind8"];
		else paramStruct.addTypeCode = addressingMap["regind16"];
		paramStruct.pcRel = true;
		paramStruct.symbol = param.substr(1, param.length() - 1);
		return paramStruct;
	}
	if (param[0] == '*') {
		paramStruct.addTypeCode = addressingMap["mem"];
		paramStruct.value = toInt(param.substr(1, param.length() - 1));
		return paramStruct;
	}
	if (param[0] == 'r' && isdigit(param[1]) ||
		((param.substr(0, 2) == "pc" || param.substr(0, 2) == "sp") && (param.length() == 2 || param[2] == '['))) {
		try { paramStruct.regCode = registerMap.at(param.substr(0, 2)); }
		catch (out_of_range) { throw SyntaxError("Unknown register: " + param.substr(0, 2)); }

		if (!byte)
			if (param.length() == 2) {
				paramStruct.addTypeCode = addressingMap["reg"];
				return paramStruct;
			}
		if (byte) {
			if (param.length() == 3) {
				switch (param[2]) {
				case 'l': paramStruct.regHigh = false; break;
				case 'h': paramStruct.regHigh = false; break;
				default: throw SyntaxError("Expected 'h' or 'l' reg specifier");
				}
				paramStruct.addTypeCode = addressingMap["reg"];
				return paramStruct;
			}
		}

		if (param[2] != '[') throw SyntaxError("'[' expected");
		if (param[param.length() - 1] != ']') throw SyntaxError("']' expected");
		param = param.substr(3, param.length() - 4);
		if (param.empty()) {
			paramStruct.addTypeCode = addressingMap["regind"];
			paramStruct.regHigh = false;
			return paramStruct;
		}
		if (isdigit(param[0]) || param[0] == '-') paramStruct.value = toInt(param);
		else paramStruct.symbol = param;
		if (byte) paramStruct.addTypeCode = addressingMap["regind8"];
		else paramStruct.addTypeCode = addressingMap["regind16"];
		return paramStruct;
	}
	if (!isdigit(param[0])) {
		paramStruct.addTypeCode = addressingMap["mem"];
		paramStruct.symbol = param;
		return paramStruct;
	}

	throw SyntaxError("Bad Parameter");
}

void InstructionParser::initMaps(){
	instructionMap.insert(pair<string, int>("halt", 0x01));
	instructionMap.insert(pair<string, int>("xchg", 0x02));
	instructionMap.insert(pair<string, int>("int", 0x03));
	instructionMap.insert(pair<string, int>("mov", 0x04));
	instructionMap.insert(pair<string, int>("add", 0x05));
	instructionMap.insert(pair<string, int>("sub", 0x06));
	instructionMap.insert(pair<string, int>("mul", 0x07));
	instructionMap.insert(pair<string, int>("div", 0x08));
	instructionMap.insert(pair<string, int>("cmp", 0x09));
	instructionMap.insert(pair<string, int>("not", 0x0A));
	instructionMap.insert(pair<string, int>("and", 0x0B));
	instructionMap.insert(pair<string, int>("or", 0x0C));
	instructionMap.insert(pair<string, int>("xor", 0x0D));
	instructionMap.insert(pair<string, int>("test", 0x0E));
	instructionMap.insert(pair<string, int>("shl", 0x0F));
	instructionMap.insert(pair<string, int>("shr", 0x10));
	instructionMap.insert(pair<string, int>("push", 0x11));
	instructionMap.insert(pair<string, int>("pop", 0x12));
	instructionMap.insert(pair<string, int>("jmp", 0x13));
	instructionMap.insert(pair<string, int>("jeq", 0x14));
	instructionMap.insert(pair<string, int>("jne", 0x15));
	instructionMap.insert(pair<string, int>("jgt", 0x16));
	instructionMap.insert(pair<string, int>("call", 0x17));
	instructionMap.insert(pair<string, int>("ret", 0x18));
	instructionMap.insert(pair<string, int>("iret", 0x19));

	addressingMap.insert(pair<string, int>("imm", 0x0));
	addressingMap.insert(pair<string, int>("reg", 0x1));
	addressingMap.insert(pair<string, int>("regind", 0x2));
	addressingMap.insert(pair<string, int>("regind8", 0x3));
	addressingMap.insert(pair<string, int>("regind16", 0x4));
	addressingMap.insert(pair<string, int>("mem", 0x5));

	registerMap.insert(pair<string, int>("r0", 0x0));
	registerMap.insert(pair<string, int>("r1", 0x1));
	registerMap.insert(pair<string, int>("r2", 0x2));
	registerMap.insert(pair<string, int>("r3", 0x3));
	registerMap.insert(pair<string, int>("r4", 0x4));
	registerMap.insert(pair<string, int>("r5", 0x5));
	registerMap.insert(pair<string, int>("r6", 0x6));
	registerMap.insert(pair<string, int>("sp", 0x6));
	registerMap.insert(pair<string, int>("r7", 0x7));
	registerMap.insert(pair<string, int>("pc", 0x7));
	registerMap.insert(pair<string, int>("psw", 0xF));

	zeroParamInstrs.push_back("halt");
	zeroParamInstrs.push_back("ret");
	zeroParamInstrs.push_back("iret");

	oneParamInstrs.push_back("int");
	oneParamInstrs.push_back("not");
	oneParamInstrs.push_back("push"); 
	oneParamInstrs.push_back("pop");
	oneParamInstrs.push_back("jmp");
	oneParamInstrs.push_back("jeq");
	oneParamInstrs.push_back("jne");
	oneParamInstrs.push_back("jgt");
	oneParamInstrs.push_back("call");
}

int  InstructionParser::getInstrSize() const {
	return instrSize;
}
