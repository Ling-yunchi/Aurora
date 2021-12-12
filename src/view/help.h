#pragma once
#include <iostream>

namespace view
{
	void help() {
		std::cout
			<< "table struct is { name, sex, email}\n"
			<< "insert : to insert a data\n"
			<< "select : to select a data\n"
			<< "update : to update a data\n"
			<< "delete : to delete a data\n"
			<< "test : to run some test\n"
			<< "exit : exit\n";
	}
	//void help() {
	//	std::cout
	//		<< "HELP:\n"
	//		<< "    tables : to show all tables\n"
	//		<< "    create <name> : to create a table\n"
	//		<< "    select <id> from <name> : get a row from table\n"
	//		<< "    insert into <name> : insert a row to table\n"
	//		<< "    update <id> from <name> : update a row from table\n"
	//		<< "    delete <id> from <name> : delete a row from table\n"
	//		<< "    test : to show some test\n";
	//}
}
