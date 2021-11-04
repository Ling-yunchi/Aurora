#pragma once
#include <xstring>

#include "LexicalAnalysis.h"

class Engine
{
public:
	Engine();

	~Engine();

	//分析指令
	void analysis(std::string& cmd);
	void print_prompt();

	void create_table();

private:
	LexicalAnalysis lexicalAnalysis;
};
