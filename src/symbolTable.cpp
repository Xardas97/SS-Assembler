#include "symbolTable.h"

SymbolTable::SymbolTable(){
	listOfSymbols = list<Symbol>();
}

Symbol* SymbolTable::find(string label) {
	for (Symbol& s : listOfSymbols) {
		if (s.label == label) return &s;
	}
	return nullptr;
}

void SymbolTable::add(string label, int section, int offset) {
	add(label, section, offset, false);
}

void SymbolTable::add(string label, int section, int offset, bool global) {
	listOfSymbols.push_back(Symbol(label, section, offset, global));
}

void SymbolTable::setGlobal(string symbol) throw(SyntaxError) {
	for (Symbol& s : listOfSymbols) {
		if (s.label == symbol) {
			s.global = true;
			return;
		}
	}
	throw SyntaxError("Symbol not defined");
}

void SymbolTable::print(ostream& out) const{
	out << "==============================================SYMBOL TABLE===============================================\n"
		<< "      Name            Section                     Offset                         G/L\n"
		<< "---------------------------------------------------------------------------------------------------------\n";
	for (Symbol s : listOfSymbols)
		out << s << "\n";
	out << "=========================================================================================================\n";
}
