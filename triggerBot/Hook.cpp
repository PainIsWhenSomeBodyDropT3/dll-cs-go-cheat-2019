#include "pch.h"
#include "Hook.h"
#include "mem.h"
bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len) {
	if (len < 5) return false;

	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	uintptr_t relativeAdress = dst - src - 5;
	*src = 0xE9;
	*(uintptr_t*)(src + 1) = relativeAdress;

	VirtualProtect(src, len, curProtection, &curProtection);
	return true;

}

bool Detour32(void* src, void* dst, int len)
{
	if (len < 5) return false;

	DWORD curProtection;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(src, 0x90, len);

	uintptr_t relativeAddress = ((uintptr_t)dst - (uintptr_t)src) - 5;

	*(BYTE*)src = 0xE9;
	*(uintptr_t*)((uintptr_t)src + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(src, len, curProtection, &temp);

	return true;
}
BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len) {
	if (len < 5) return 0;


	BYTE* gateway = (BYTE*)VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	memcpy_s(gateway, len, src, len);

	uintptr_t gatewayRelativeAddress = src - gateway - 5;

	*(gateway + len) = 0xE9;

	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gatewayRelativeAddress;

	Detour32(src, dst, len);

	return gateway;


}

Hook::Hook(void* src, void* dst, uintptr_t len) {
	this->srcV = src;
	this->dstV = dst;
	this->len = len;
}
Hook::Hook(BYTE* src, BYTE* dst, BYTE* PrtToGateWay, uintptr_t len) {
	this->src = src;
	this->dst = dst;
	this->PrtToGateWay = PrtToGateWay;
	this->len = len;
}

Hook::Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PrtToGateWay, uintptr_t len) {

	HMODULE hMod = GetModuleHandleA(modName);

	this->src = (BYTE*)GetProcAddress(hMod, exportName);
	this->dst = dst;
	this->PrtToGateWay = PrtToGateWay;
	this->len = len;
}

void Hook::Enable() {
	memcpy(originalByte, src, len);
	*(uintptr_t*)PrtToGateWay = (uintptr_t)TrampHook32(src, dst, len);
	bStatus = true;
}
void  Hook::EnableMiddle() {
	memcpy(originalByte, srcV, len);
	
	Detour32(srcV, dstV, len);
	bStatus = true;
}
void Hook::Disable() {
	mem::Patch(src, originalByte, len);
	bStatus = false;
}
void Hook::DisableMiddle() {
	mem::Patch((BYTE*)srcV, originalByte, len);
	bStatus = false;
}
void Hook::Toggle() {
	if (!bStatus) {
		Enable();
	}
	else {
		Disable();
	}
}