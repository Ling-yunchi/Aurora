#include <iostream>
#include <string>

#include "BTree.h"
#include "Engine.h"
using namespace std;

int main()
{
	//string cmd;
	//Engine engine;
	//while (true) {
	//	engine.print_prompt();
	//	getline(cin, cmd);
	//	engine.analysis(cmd);
	//}
	BTree b_tree;
	for (int i = 0; i < 10000; i++)
		b_tree.insert(i, new Data(i));

	b_tree.ldr();
	
	return 0;
}
