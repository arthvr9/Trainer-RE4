#include <windows.h>
#include <iostream>
#include <psapi.h>
#include "../memory/Memory.h"

class InfiniteAmmo {
	private:
		uintptr_t base;
		Memory* memory;

	public: 
		InfiniteAmmo(uintptr_t baseAddr, Memory* mem);
		void Update();
};

