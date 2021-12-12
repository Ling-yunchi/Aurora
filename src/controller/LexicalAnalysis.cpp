#include "LexicalAnalysis.h"

#include <iostream>

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

LexicalAnalysis::LexicalAnalysis() :legal(true) {}

void LexicalAnalysis::syntax_error() {
	cout << "Syntax error" << endl;
}

void LexicalAnalysis::analysis()
{
	legal = true;
	vector<string> token = split(str, ' ');
	//for (int i = 0; i < token.size(); i++) {
	//	std::cout << "[" << i << "]" << token[i] << " ";
	//}
	//if (!token.empty()) cout << endl;
	if (!token.empty()) {
		if (token[0] == "help") keyword = operation::help_;
		//else if (token[0] == "tables") keyword = operation::tables_;
		//else if (token[0] == "create") keyword = operation::create_;
		else if (token[0] == "select") keyword = operation::select_;
		else if (token[0] == "insert") keyword = operation::insert_;
		else if (token[0] == "update") keyword = operation::update_;
		else if (token[0] == "delete") keyword = operation::delete_;
		else if (token[0] == "test") keyword = operation::test_;
		else if (token[0] == "cls") {
			system("cls");
			legal = false;
		}
		else {
			legal = false;
			syntax_error();
		}
	}
	else legal = false;
	if (!token.empty())
		parameter.insert(parameter.begin(), ++token.begin(), token.end());
}

