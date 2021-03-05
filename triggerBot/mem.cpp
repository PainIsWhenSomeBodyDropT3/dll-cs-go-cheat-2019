#include "pch.h"
#include "mem.h"
void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	DWORD oldProject;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldProject);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldProject, &oldProject);

}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess) {
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
}

uintptr_t mem::FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldProject;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProject);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldProject, &oldProject);

}

void mem::Nop(BYTE* dst, unsigned int size) {
	DWORD oldProject;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProject);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldProject, &oldProject);
}

uintptr_t mem::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}
