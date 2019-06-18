#include "Interpreter.h"


int main() {
	Interpreter cli;
	std::string input;
	std::cout << "Welcome to CLI Interpreter!" << std::endl;
	while (std::getline(std::cin, input)) {
		if (input == "quit") {
			exit(1);
		}
		cli.LexAnalysis(input);
		cli.Parse();
	}
	system("pause");

};
