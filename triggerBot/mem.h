#pragma once
#pragma once
#include "pch.h"
namespace mem
{
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
	void Nop(BYTE* dst, unsigned int size);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
	uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

	
}
