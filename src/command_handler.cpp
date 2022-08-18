#include <vector>
#include <iostream>

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <Psapi.h>
#include <string>
#include <sstream>
#include <stdio.h>

#include "program_vars.h"

void handle_command(ProgramVars* vars, std::vector<std::string> args) {
	std::string command_name = args.front();

	if (command_name == "info") {
		std::cout << "Info command!\n";
	}
	if (command_name == "ps") {
		DWORD pIDs[1024];
		DWORD cbNeeded;
		if (!EnumProcesses(pIDs, sizeof(pIDs), &cbNeeded)) { return; };
		DWORD pCount = cbNeeded / sizeof(DWORD);
		for (int i = 0; i < pCount; i++) {
			DWORD pID = pIDs[i];
			HANDLE pHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pID);
			HMODULE pModule;
			DWORD cb;
			TCHAR pName[MAX_PATH] = TEXT("<unknown>");

			if (!EnumProcessModulesEx(pHandle, &pModule, sizeof(HMODULE), &cb, LIST_MODULES_ALL)) 
			{ continue; };			
			if (!GetModuleBaseName(pHandle, pModule, pName, sizeof(pName)/sizeof(TCHAR))) 
			{ continue; };

			std::wcout << pID << ": " << pName << "\n";

			CloseHandle(pHandle);
		}
	}
	if (command_name == "modules") {
		HMODULE modules[1024];
		DWORD cbNeeded;
		
		if (!EnumProcessModulesEx(vars->pHandle, modules, sizeof(modules), &cbNeeded, LIST_MODULES_ALL)) {
			std::cout << "failed to enum process modules (missing PROCESS_VM_READ?)\n";
			return;
		}
		
		DWORD modulesCount = cbNeeded / sizeof(HMODULE);
		
		for (int i = 0; i < modulesCount; i++) {
			HMODULE pModule = modules[i];
			TCHAR moduleName[MAX_PATH] = TEXT("<unknown>");
			
			if (!GetModuleBaseName(vars->pHandle, pModule, moduleName, sizeof(moduleName) / sizeof(TCHAR))) 
			{ continue; }
			
			std::wcout << "| " << moduleName << "\n";
		}

	}
	if (command_name == "open_process") {
		if (args.size() < 2) {
			std::cout << "[!] Missing process ID\n";
			return;
		}
		DWORD pid = (DWORD)stoi(args[1]);
		std::cout << "-> Opening process ID: " << pid << "\n";
		DWORD desiredAccess = PROCESS_QUERY_INFORMATION;
		std::cout << "\t.. with PROCESS_QUERY_INFORMATION";
		if (args.size() > 2) {
			std::stringstream ss(args[2]);
			std::string access;
			while (std::getline(ss, access, ',')) {
				if (access == "PROCESS_ALL_ACCESS") desiredAccess |= PROCESS_ALL_ACCESS;
				else if (access == "PROCESS_VM_READ") desiredAccess |= PROCESS_VM_READ;
				else {
					std::cout << "\n\tIgnoring unsupported access: " << access << "\n";
					continue;
				}
				std::cout << ", " << access;
			}
		}
		std::cout << "\n";
		
		CloseHandle(vars->pHandle);
		vars->pHandle = OpenProcess(desiredAccess, FALSE, pid);
		
		if (vars->pHandle == NULL) {
			std::cout << "[!] Couldn't open process: " << GetLastError() << "\n";
			vars->pHandle = GetCurrentProcess();
		}
	}
	if (command_name == "exit") {
		CloseHandle(vars->pHandle);
		exit(0);
	}
}