#pragma once

#include<string>

using namespace std;

class SyntaxError {
public:
	SyntaxError(string message) { this->message = message; }
	SyntaxError() { SyntaxError(""); }
	string getMessage() const { return message; }
private:
	string message;
};