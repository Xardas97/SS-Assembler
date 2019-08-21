#include<string>
#include<iomanip>
#include<sstream>
#include<cmath>

using namespace std;

string swapBytes(string str) {
	if (str.length() != 4) return str;
	string result = "";
	result.push_back(str[2]);
	result.push_back(str[3]);
	result.push_back(str[0]);
	result.push_back(str[1]);
	return result;
}

string format(string str) {
	string result = "";
	int breakPoint = 68;
	for (int i = 0; i < str.length(); i++, breakPoint--) {
		result += str[i];
		if (i % 2)
			if (breakPoint < 0) { result += "\n"; breakPoint = 68; }
			else result += " ";
	}
	return result;
}

int toInt(string str) {
	return strtol(str.c_str(), 0, 0);
}

int toInt(string str, int base) {
	return strtol(str.c_str(), 0, base);
}

string toHex(unsigned long long value, int byteSize) {
	value = value % (int)pow(2, byteSize * 8);

	stringstream stream;
	stream << std::hex << value;
	string result(stream.str());

	int missingChars = byteSize * 2 - result.length();
	if (missingChars > 0)
		for (int i = 0; i < missingChars; i++)
			result = "0" + result;

	return result;
}
