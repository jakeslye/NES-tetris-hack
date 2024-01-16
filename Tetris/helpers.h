#pragma once
#include <windows.h>
#include <TlHelp32.h>
#include "globals.h"
#include <vector>

DWORD GetProcId(const wchar_t* procName);
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
uintptr_t FindDMAaddy(uintptr_t ptr, std::vector<unsigned int> offsets);

template <typename T>
T Read(uintptr_t addr) {
	T value;
	ReadProcessMemory(handle, (LPCVOID)addr, &value, sizeof(T), NULL);
	return value;
}

template <typename T>
void Write(uintptr_t addr, T value) {
	WriteProcessMemory(handle, (LPVOID)addr, &value, sizeof(T), NULL);
}