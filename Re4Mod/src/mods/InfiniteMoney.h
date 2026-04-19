#pragma once
#include <iostream>
#include <Windows.h>
#include "../memory/Memory.h"

class InfiniteMoney {
	private:
		uintptr_t base;
		Memory* memory;

	public:
		InfiniteMoney(uintptr_t baseAddr, Memory* mem);
		void Update();
};

