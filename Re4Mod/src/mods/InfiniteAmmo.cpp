#include "InfiniteAmmo.h"

InfiniteAmmo::InfiniteAmmo(uintptr_t baseAddr, Memory* mem) {
	base = baseAddr;
	memory = mem;
}

// 15 bullets on red9: 7877155

void InfiniteAmmo::Update() {
	if (base == 0)
		return;

	uintptr_t secondPos = base + 0x00870FE8;

	DWORD thirdPos = memory->Read<DWORD>(secondPos);
	if (thirdPos == 0)
		return;

	uintptr_t finalAddr = static_cast<uintptr_t>(thirdPos) + 0x1D4;

	int value = 7877155;

	memory->Write<int>(finalAddr, value);
}