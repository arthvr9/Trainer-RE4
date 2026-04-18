#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

class Process
{
public:
	DWORD GetProcessId(const wchar_t* processName);
};

