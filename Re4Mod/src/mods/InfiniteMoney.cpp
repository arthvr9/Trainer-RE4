#include "InfiniteMoney.h"
#include <windows.h>

void InfiniteMoney::Update(HANDLE hProcess) {
	DWORD money_value = 999999999;
	WriteProcessMemory(hProcess, (LPVOID)money_addr, &money_value, sizeof(money_value), NULL);
}
