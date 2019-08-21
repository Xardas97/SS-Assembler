#include"assembler.h"
#include<iostream>

int main(int argc, char** argv) {
	if (argc != 4) return -1;

	Assembler as = Assembler(argv[3], argv[2]);
	
	auto firstPassOutput = as.firstPass();
	as.secondPass(firstPassOutput);

	return 0;
}