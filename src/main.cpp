#include <iostream>
#include <vector>
#include <string>

#include <Windows.h>
#include <Psapi.h>

#include "command_handler.h"
#include "program_vars.h"

void print_prompt(ProgramVars* args) {
	HMODULE baseModule;
	DWORD cb;
	TCHAR pName[MAX_PATH] = TEXT("<unknown>");

	std::cout << "(";

	if (EnumProcessModulesEx(args->pHandle, &baseModule, sizeof(HMODULE), &cb, LIST_MODULES_ALL)) {
		if (GetModuleBaseName(args->pHandle, baseModule, pName, sizeof(pName) / sizeof(TCHAR))) {
			std::wcout << pName;
		}
		else {
			std::cout << "PID " << GetProcessId(args->pHandle);
		}
	}
	else {
		std::cout << "PID " << GetProcessId(args->pHandle);
	}
	std::cout << ") PM > ";
}

int main() {
	ProgramVars program;
	program.pHandle = GetCurrentProcess();

	while (true) {
		print_prompt(&program);

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
			else { cur_string += cur_char; }
		}

		handle_command(&program, args);
	}

	return 0;
}