#include <iostream>
#include <string>

#include "Engine.h"
using namespace std;

int main()
{
	string cmd;
	Engine engine;
	while (true) {
		engine.print_prompt();
		getline(cin, cmd);
		engine.analysis(cmd);
	}

}
