#include <iostream>
#include <windows.h>
#include <psapi.h>
#include "process/Process.h"
#include "memory/Memory.h"
#include "mods/InfiniteAmmo.h"

int main()
{
	Process proc;

	if (!proc.Attach(L"bio4.exe")) {
		printf("processo não encontrado.\n");
		return 1;
	}

	uintptr_t base = proc.GetModuleBase(L"bio4.exe");

	Memory mem(proc.handle);
	InfiniteAmmo ammo(base, &mem);

	while (true) {
		ammo.Update();
		Sleep(100); // atualiza a cada 100ms
	}
}
