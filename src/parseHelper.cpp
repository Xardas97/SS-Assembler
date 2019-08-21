#include "parseHelper.h"

ParseHelper::ParseHelper(string line) throw(SyntaxError) {
	int pos;

	//cut spaces in the start of the line
	if ((pos = line.find_first_not_of(" \t")) == string::npos) throw SyntaxError("Missing Parameter");
	line = line.substr(pos, line.length() - pos);

	while (true) {
		//first parameter ends with the first space
		if ((pos = line.find_first_of(" ,\t")) == string::npos) params.push_back(line);
		else params.push_back(line.substr(0, pos));

		//cut spaces until the start of the second param if there is a second parameter
		if ((pos = line.find_first_of(",")) == string::npos) return;
		line = line.substr(pos + 1, line.length() - pos - 1);
		if ((pos = line.find_first_not_of(" \t")) == string::npos) throw SyntaxError("Missing Parameter");
		line = line.substr(pos, line.length() - pos);
	}
}

vector<string> ParseHelper::getParams() const {
	return params;
}

string ParseHelper::getFirstParam() const {
	if (params.size() < 1) return "";
	return params[0];
}

string ParseHelper::getSecondParam() const {
	if (params.size() < 2) return "";
	return params[1];
}