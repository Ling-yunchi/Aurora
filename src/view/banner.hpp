#pragma once
#define RESET "\033[0m"
#define BLACK "\033[30m" /* Black */
#define RED "\033[31m" /* Red */
#define GREEN "\033[32m" /* Green */
#define YELLOW "\033[33m" /* Yellow */
#define BLUE "\033[34m" /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m" /* Cyan */
#define WHITE "\033[37m" /* White */
#define BOLDBLACK "\033[1m\033[30m" /* Bold Black */
#define BOLDRED "\033[1m\033[31m" /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m" /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m" /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m" /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m" /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m" /* Bold White */

#include <iostream>
#include <random>

namespace view
{
	void banner1() {
		std::cout
			<< RED << "     /\\                                  \n" << RESET
			<< YELLOW << "    /  \\   _   _  _ __  ___   _ __  __ _ \n" << RESET
			<< GREEN << "   / /\\ \\ | | | || '__|/ _ \\ | '__|/ _` |\n" << RESET
			<< MAGENTA << "  / ____ \\| |_| || |  | (_) || |  | (_| |\n" << RESET
			<< CYAN << " /_/    \\_\\\\__,_||_|   \\___/ |_|   \\__,_|\n" << RESET;

	}
	void banner2() {
		std::cout
			<< RED << "    ___                                   \n" << RESET
			<< YELLOW << "   /   |  __  __ _____ ____   _____ ____ _\n" << RESET
			<< GREEN << "  / /| | / / / // ___// __ \\ / ___// __ `/\n" << RESET
			<< MAGENTA << " / ___ |/ /_/ // /   / /_/ // /   / /_/ / \n" << RESET
			<< CYAN << "/_/  |_|\\__,_//_/    \\____//_/    \\__,_/  \n" << RESET;
	}
	void banner3() {
		std::cout
			<< RED << "      ___         ___         ___         ___         ___         ___     \n" << RESET
			<< YELLOW << "     /  /\\       /__/\\       /  /\\       /  /\\       /  /\\       /  /\\    \n" << RESET
			<< GREEN << "    /  /::\\      \\  \\:\\     /  /::\\     /  /::\\     /  /::\\     /  /::\\   \n" << RESET
			<< MAGENTA << "   /  /:/\\:\\      \\  \\:\\   /  /:/\\:\\   /  /:/\\:\\   /  /:/\\:\\   /  /:/\\:\\  \n" << RESET
			<< CYAN << "  /  /:/~/::\\ ___  \\  \\:\\ /  /:/~/:/  /  /:/  \\:\\ /  /:/~/:/  /  /:/~/::\\ \n" << RESET
			<< RED << " /__/:/ /:/\\:/__/\\  \\__\\:/__/:/ /:/__/__/:/ \\__\\:/__/:/ /:/__/__/:/ /:/\\:\\\n" << RESET
			<< YELLOW << " \\  \\:\\/:/__\\\\  \\:\\ /  /:\\  \\:\\/:::::\\  \\:\\ /  /:\\  \\:\\/:::::\\  \\:\\/:/__\\/\n" << RESET
			<< GREEN << "  \\  \\::/     \\  \\:\\  /:/ \\  \\::/~~~~ \\  \\:\\  /:/ \\  \\::/~~~~ \\  \\::/     \n" << RESET
			<< MAGENTA << "   \\  \\:\\      \\  \\:\\/:/   \\  \\:\\      \\  \\:\\/:/   \\  \\:\\      \\  \\:\\     \n" << RESET
			<< CYAN << "    \\  \\:\\      \\  \\::/     \\  \\:\\      \\  \\::/     \\  \\:\\      \\  \\:\\    \n" << RESET
			<< RED << "     \\__\\/       \\__\\/       \\__\\/       \\__\\/       \\__\\/       \\__\\/    \n" << RESET;
	}
	void banner4() {
		std::cout
			<< RED << " ________  ___  ___  ________  ________  ________  ________     \n" << RESET
			<< YELLOW << "|\\   __  \\|\\  \\|\\  \\|\\   __  \\|\\   __  \\|\\   __  \\|\\   __  \\    \n" << RESET
			<< GREEN << "\\ \\  \\|\\  \\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\|\\  \\   \n" << RESET
			<< MAGENTA << " \\ \\   __  \\ \\  \\\\\\  \\ \\   _  _\\ \\  \\\\\\  \\ \\   _  _\\ \\   __  \\  \n" << RESET
			<< CYAN << "  \\ \\  \\ \\  \\ \\  \\\\\\  \\ \\  \\\\  \\\\ \\  \\\\\\  \\ \\  \\\\  \\\\ \\  \\ \\  \\ \n" << RESET
			<< RED << "   \\ \\__\\ \\__\\ \\_______\\ \\__\\\\ _\\\\ \\_______\\ \\__\\\\ _\\\\ \\__\\ \\__\\\n" << RESET
			<< YELLOW << "    \\|__|\\|__|\\|_______|\\|__|\\|__|\\|_______|\\|__|\\|__|\\|__|\\|__|\n" << RESET;
	}
	void banner() {
		std::default_random_engine e(static_cast<unsigned int>(time(nullptr)));
		std::uniform_int<int> rand_int(1, 4);
		int n = rand_int(e);
		switch (n) {
		case 1:banner1(); break;
		case 2:banner2(); break;
		case 3://banner3(); break;
		case 4:banner4(); break;
		default: break;
		}
		std::cout << BOLDWHITE << "                 version 0.0.0     by.wr\n" << RESET;
	}

	void tip() {
		std::cout << "Try: help for more help\n";
	}
}
