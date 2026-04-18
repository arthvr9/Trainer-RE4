#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

class Process
{
public:
	DWORD pid;
	HANDLE handle;

	bool Attach(const wchar_t* processName);
	uintptr_t GetModuleBase(const wchar_t* moduleName);

private:
	DWORD GetProcessId(const wchar_t* processName);
};

