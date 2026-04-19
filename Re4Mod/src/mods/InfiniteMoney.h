#pragma once
#include <Windows.h>

class InfiniteMoney
{
	DWORD money_addr = 0x00FEF708; // TODO: find if this address is static or if it needs to be calculated

	public:
		void Update(HANDLE hProcess);
};

