#include "Memory.h"
#include <windows.h>
#include <cstdint>

Memory::Memory(HANDLE h) {
	handle = h;
}

template<typename T>
T Memory::Read(uintptr_t address) {
	T value{};
	ReadProcessMemory(handle, (LPCVOID)address, &value, sizeof(T), NULL);]
	return value;
}

template<typename T>
void Memory::Write(uintptr_t address, T value) {
	WriteProcessMemory(handle, (LPVOID)address, &value, sizeof(T), NULL);
}