#include "LexicalAnalysis.h"

vector<string> split(const string& str, const char c);

LexicalAnalysis::LexicalAnalysis() :legal(true) {}

void LexicalAnalysis::analysis()
{
	vector<string> token = split(str, ' ');
	if (!token.empty()) {
		if (token[0] == "createtable") keyword = operation::create_table_;
		else if (token[0] == "select") keyword = operation::select_;
		else if (token[0] == "insert") keyword = operation::insert_;
		else if (token[0] == "update") keyword = operation::update_;
		else if (token[0] == "delete") keyword = operation::delete_;
		else legal = false;
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