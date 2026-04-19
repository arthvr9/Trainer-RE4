#include "InfiniteMoney.h"
#include <windows.h>

InfiniteMoney::InfiniteMoney(uintptr_t baseAddr, Memory* mem) {
	base = baseAddr;
	memory = mem;
}

void InfiniteMoney::Update() {
	DWORD money_value = 999999999;

	if (base == 0)
		return;

	uintptr_t secondPos = base + 0x002B1E4C;

	DWORD thirdPos = memory->Read<DWORD>(secondPos);
	if (thirdPos == 0)
		return;

	uintptr_t finalAddr = static_cast<uintptr_t>(thirdPos) + 0x7C;

	memory->Write<int>(finalAddr, money_value);
}
