#include <iostream>
#include <windows.h>
#include <psapi.h>

int main()
{
	// may variate
	// TODO: set dynamic
	const int PID = 17928;
	DWORD ammo = 0x08EB7574;
	int valor = 15728640;

	// dinheiro: 00FFDF708

	HANDLE processoRe4 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	HMODULE re4Module = NULL;
	DWORD nullable;

	if (processoRe4 == NULL) {
		printf("erro ao abrir processo. codigo: %lu\n", GetLastError());
		exit(1);
	}

	if (EnumProcessModules(processoRe4, &re4Module, sizeof(re4Module), &nullable)) {
		MODULEINFO info;

		if (GetModuleInformation(processoRe4, re4Module, &info, sizeof(info))) {
			DWORD memoriaBase = (DWORD)info.lpBaseOfDll;
			DWORD secondPos = memoriaBase + 0x00870FE8;
			DWORD thirdPos;

			if (ReadProcessMemory(processoRe4, (LPCVOID)secondPos, &thirdPos, sizeof(thirdPos), NULL)) {
				int* ptr = (int*)thirdPos;
				ptr = (int*)((char*)ptr + 0x1D4);

				while (1) {
					if (WriteProcessMemory(processoRe4, ptr, &valor, sizeof(valor), NULL)) {
						printf("valor escrito com sucesso!\n");

					}
					else {
						printf("erro ao escrever valor. codigo: %lu\n", GetLastError());
						exit(1);
					}
					Sleep(100);
				}

				printf("valor lido com sucesso: %p\n", ptr);

			}
			else {
				printf("erro ao ler valor. codigo: %lu\n", GetLastError());
				exit(1);
			}
		}
		else {
			printf("erro ao buscar info do modulo");
			exit(1);
		}
	}
}
