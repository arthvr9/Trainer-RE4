#include "InfiniteAmmo.h"

InfiniteAmmo::InfiniteAmmo(uintptr_t baseAddr, Memory* mem) {
	base = baseAddr;
	memory = mem;
}

void InfiniteAmmo::Update() {
	uintptr_t secondPos = base + 0x00870FE8;

	uintptr_t thirdPos = memory->Read<uintptr_t>(secondPos);

	uintptr_t finalAddr = thirdPos + 0x1D4;

	int value = 15728640; // 30 bullets

	memory->Write<int>(finalAddr, value);
}