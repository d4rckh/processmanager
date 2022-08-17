#include <iostream>
#include <vector>
#include <string>

#include "command_handler.h"

int main() {
	while (true) {
		std::cout << "> ";

		std::vector<std::string> args;
		
		std::string cur_string = "";
		char cur_char = ' ';
		
		while (true) {
			cur_char = getchar();
			if (cur_char == ' ' || cur_char == '\n') {
				args.push_back(cur_string);
				cur_string = "";
				if (cur_char == '\n') { break; }
			}
			else { cur_string += cur_char;  }
		}

		handle_command(args);
	}

	return 0;
}