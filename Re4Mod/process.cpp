#include "process.h"
#include <tlhelp32.h>
#include <iostream>

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