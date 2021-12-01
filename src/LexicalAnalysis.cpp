#include "LexicalAnalysis.h"

vector<string> split(const string& str, const char c);

LexicalAnalysis::LexicalAnalysis() :isLegal(true) {}

void LexicalAnalysis::analysis()
{
	vector<string> token = split(str,' ');
	if(!token.empty()) {
		if (token[0] == "createtable") keyword = operation::create_table_;
		else if (token[0] == "select") keyword = operation::select_;
	}
}

vector<string> split(const string& str, const char c) {
	vector<string> result;
	int lastpos = str.find_first_not_of(c, 0);
	int pos = str.find_first_of(c, lastpos);
	while (pos != string::npos || lastpos != string::npos) {
		result.push_back(str.substr(lastpos, pos - lastpos));
		lastpos = str.find_first_not_of(c, pos);
		pos = str.find_first_of(c, lastpos);
	}
	return result;
}