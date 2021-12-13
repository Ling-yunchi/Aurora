#include <iostream>
#include "view/banner.hpp"
#include "test/test.hpp"

//#define TEST
using namespace std;

int main()
{
#ifdef TEST
	test::test_entry();
#endif

#ifndef TEST
	string cmd;
	Engine engine;
	system("cls");
	view::banner();
	view::tip();
	while (true) {
		engine.print_prompt();
		getline(cin, cmd);
		if (cmd == "exit") break;
		engine.analysis(cmd);
	}
#endif
	return 0;
}
