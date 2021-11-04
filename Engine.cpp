#include "Engine.h"
#include <iostream>

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::analysis(std::string& cmd)
{
	cout << (cmd.empty() ? "" : cmd + "\n");
	lexicalAnalysis.set_str(cmd);
	lexicalAnalysis.analysis();
	if(lexicalAnalysis.is_legal()) {
		switch (lexicalAnalysis.get_keyword()) {
		case create_table_: create_table(); break;
		case insert_: break;
		case select_: break;
		case update_: break;
		case delete_: break;
		default: ;
		}
	}
}

void Engine::print_prompt()
{
	cout << "sqlwr > ";
}

void Engine::create_table()
{
}
