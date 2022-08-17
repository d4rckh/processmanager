#include <vector>
#include <iostream>

void handle_command(std::vector<std::string> args) {
	std::string command_name = args.front();

	if (command_name == "info") {
		std::cout << "Info command!\n";
	}
}