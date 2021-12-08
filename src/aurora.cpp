#include <iostream>
#include "test/test.hpp"

#define TEST
using namespace std;

int main()
{
#ifdef TEST
	test::test_entry();
#endif

	//string cmd;
	//Engine engine;
	//while (true) {
	//
	//	engine.print_prompt();
	//	getline(cin, cmd);
	//	engine.analysis(cmd);
	//}

	return 0;
}
