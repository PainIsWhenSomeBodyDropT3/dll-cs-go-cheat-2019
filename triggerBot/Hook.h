#pragma once
bool Detour32(BYTE* src, BYTE* dst, const uintptr_t len);
bool Detour32(void* src, void* dst, int len);
BYTE* TrampHook32(BYTE* src, BYTE* dst, const uintptr_t len);
class Hook
{
public:
	bool bStatus{ false };

	BYTE* dst{ nullptr };
	void* dstV{ nullptr };
	BYTE* src{ nullptr };
	void* srcV{ nullptr };
	BYTE* PrtToGateWay{ nullptr };
	uintptr_t len{ 0 };

	BYTE originalByte[10]{ 0 };


	Hook(void* src, void* dst, uintptr_t len);
	Hook(BYTE* src, BYTE* dst, BYTE* PrtToGateWay, uintptr_t len);
	Hook(const char* exportName, const char* modName, BYTE* dst, BYTE* PrtToGateWay, uintptr_t len);

	void Enable();
	void EnableMiddle();
	void Disable();
	void DisableMiddle();
	void Toggle();
};

