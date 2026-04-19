#include "Process.h"
#include <tlhelp32.h>
#include <iostream>
#include <psapi.h>

bool Process::Attach(const wchar_t* processName) {
	pid = GetProcessId(processName);

	if (pid == 0) {
		printf("Não foi possível encontrar o processo.\n");
		return false;
	}

	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	return handle != NULL;
}

DWORD Process::GetProcessId(const wchar_t* processName) {
	PROCESSENTRY32W entry;
	entry.dwSize = sizeof(PROCESSENTRY32W);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32FirstW(snapshot, &entry)) {
		do {
			if (!_wcsicmp(entry.szExeFile, processName)) {
				CloseHandle(snapshot);
				return entry.th32ProcessID;
			}
		} while (Process32NextW(snapshot, &entry));
	}

	CloseHandle(snapshot);
	return 0;
}

uintptr_t Process::GetModuleBase(const wchar_t* moduleName) {
 HMODULE modules[1024];
	DWORD needed = 0;

	if (EnumProcessModules(handle, modules, sizeof(modules), &needed)) {
		const DWORD count = needed / sizeof(HMODULE);
		for (DWORD i = 0; i < count; ++i) {
			wchar_t name[MAX_PATH]{};
			if (GetModuleBaseNameW(handle, modules[i], name, MAX_PATH) == 0)
				continue;

			if (_wcsicmp(name, moduleName) == 0) {
				MODULEINFO info{};
				if (GetModuleInformation(handle, modules[i], &info, sizeof(info))) {
					return (uintptr_t)info.lpBaseOfDll;
				}
			}
		}
	}

	return 0;
}