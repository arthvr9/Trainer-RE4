#include "Memory.h"
#include <windows.h>
#include <cstdint>

Memory::Memory(HANDLE h) {
	handle = h;
}
