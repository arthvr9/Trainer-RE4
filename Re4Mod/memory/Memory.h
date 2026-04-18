#pragma once
#include <windows.h>
#include <cstdint>

class Memory {
private:
	HANDLE handle;

public:
	Memory(HANDLE h);

	template<typename T>
	T Read(uintptr_t address);

	template<typename T>
	void Write(uintptr_t address, T value);
};

