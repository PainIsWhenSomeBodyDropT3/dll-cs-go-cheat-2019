// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "proc.h"
#include "mem.h"
#include "triger.h"
#include "Aim.h"
#include "jump.h"
#include "csgo.h"
#include "Hook.h"
//#include "drawHook.h"
#include "includes.h"
#include <iostream>
#include <exception>
#include <ctime>
#include <Psapi.h>

int count = 7;
uintptr_t moduleBase = (uintptr_t)(GetModuleHandle(L"client.dll"));

uintptr_t extraModuleBase = (uintptr_t)(GetModuleHandle(L"engine.dll"));

CBaseEntList* entList = (CBaseEntList*)(moduleBase + offsets::dwEntityList);

Ent* localPlayer = *(Ent**)(moduleBase + offsets::dwLocalPlayer);

ExtraEnt* extraLocalPlayer = *(ExtraEnt**)(extraModuleBase + offsets::dwClientState);

LBA* lba = (LBA*)(moduleBase + offsets::dwForceAttack);

RBA* rba = (RBA*)(moduleBase + offsets::dwForceAttack2);

Jump* jump = (Jump*)(moduleBase + offsets::dwForceJump);

float* viewMathrix = (float*)(moduleBase + offsets::dwViewMatrix);

Forward* forward = (Forward*)(moduleBase + offsets::dwForceForward);

Backward* backward = (Backward*)(moduleBase + offsets::dwForceBackward);

Left* left = (Left*)(moduleBase + offsets::dwForceLeft);

Right* right = (Right*)(moduleBase + offsets::dwForceRight);

typedef void* (__cdecl* tCreateInterface)(const char* name, int* pReturnCode);

void* GetInterface(tCreateInterface fn, const char* name)
{
	return (fn)(name, 0);
}
tTraceRay TraceRay;
tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress((HMODULE)extraModuleBase, "CreateInterface");
IEngineTrace* EngineTrace = (IEngineTrace*)GetInterface(CreateInterface, "EngineTraceClient004");

Aim aimBot = Aim(moduleBase, entList, localPlayer, extraLocalPlayer);
trigger triggerBot = trigger(moduleBase, entList, localPlayer, lba, rba);

void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

//#define ASM_OPCODE_JMP_REL        0xEB
//#define ASM_OPCODE_NOP            0x90
//// size of the full assembler instruction in bytes
//#define ASM_INSTR_SIZE_JMP_REL    0x2
//#define ASM_INSTR_SIZE_PUSH        0x1
//#define ASM_INSTR_SIZE_NOP        0x1
//// length of your pattern in bytes
//#define MARKER_SIZE 14
//// the pattern of the marker and its mask
//#define MARKER_BYTES "\x9C\x60\x33\xC0\x33\xC0\x33\xDB\x33\xC0\x33\xC0\x61\x9D"
//// I know the mask is pretty useless in this case but I like to use a generic findPattern method which can handle every mask
//#define MARKER_MASK "xxxxxxxxxxxxxx"
//
//MODULEINFO GetModuleInfo(const char* modulename) {
//	MODULEINFO modInfo = { 0 };
//	HMODULE hModule = GetModuleHandleA(modulename);
//	if (hModule == 0) {
//		return modInfo;
//	}
//	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
//	
//	//std::cout << modInfo;
//	return modInfo;
//}
//
//DWORD FindPattern(const char* module, const char* pattern, const char* mask) {
//	MODULEINFO mInfo = GetModuleInfo(module);
//
//
//
//	DWORD base = (DWORD)mInfo.lpBaseOfDll;
//	DWORD size = (DWORD)mInfo.SizeOfImage;
//	//std::cout << base;
//
//	DWORD patternLength = (DWORD)strlen(mask);
//
//	for (DWORD i = 0; i < size - patternLength; i++) {
//		bool found = true;
//
//		for (DWORD j = 0; j < patternLength; j++) {
//			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
//		}
//		if (found) {
//			return base + i;
//		}
//	}
//	return NULL;
//}
//
//void morphMemory(BYTE* pbyDst, BYTE byLength)
//{
//	/*
//	what kind of junk code this algorythm can produce (rdm = random opcode, jmp = jump past junk code)
//	jmp        or        0x90
//	rdm                jmp
//	rdm                rdm
//	rdm                rdm
//	*/
//
//	// nullptr checking would be a good idea at this point
//	// init random number generator only once
//	static bool bSetSeed = true;
//	if (bSetSeed)
//	{
//		srand((UINT)time(NULL));
//		bSetSeed = false;
//	}
//
//	// get a place for our brand new random opcodes
//	BYTE* morphedOpcodes = new BYTE[byLength];
//	BYTE byOpcodeIt = 0;
//
//	// first calculate if we wanna place a 0x90 at the beginning or not
//	bool bPlaceNop = (rand() % 2) ? true : false;
//	if (bPlaceNop)
//	{
//		morphedOpcodes[byOpcodeIt] = ASM_OPCODE_NOP;
//		byOpcodeIt++;
//	}
//
//	// now place the jmp instruction
//	morphedOpcodes[byOpcodeIt] = ASM_OPCODE_JMP_REL;
//	byOpcodeIt++;
//
//	// adjust the relative jmp based on wether we placed a nop before or not
//	// keep in mind: we only want to jump past the junk code not any further
//	morphedOpcodes[byOpcodeIt] = byLength - ASM_INSTR_SIZE_JMP_REL - ((bPlaceNop) ? ASM_INSTR_SIZE_NOP : 0);
//	byOpcodeIt++;
//
//	// now fill the rest with random opcodes
//	for (; byOpcodeIt < byLength; byOpcodeIt++)
//		morphedOpcodes[byOpcodeIt] = rand() % MAXBYTE; // 0xFF
//
//	// write the new opcodes to memory
//	DWORD dwOldProtect;
//	VirtualProtect(pbyDst, byLength, PAGE_EXECUTE_READWRITE, &dwOldProtect);
//	memcpy(pbyDst, morphedOpcodes, byLength);
//	VirtualProtect(pbyDst, byLength, dwOldProtect, &dwOldProtect);
//
//	// free the used memory again
//	delete morphedOpcodes;
//}
//void morphModule()
//{
//	// all the vars we need for the GetModuleInformation call
//	MODULEINFO modInfo;
//	HANDLE hProcess = GetCurrentProcess();
//	HMODULE hModule = GetModuleHandleA(NULL);
//
//	if (GetModuleInformation(hProcess, hModule, &modInfo, sizeof(MODULEINFO)))
//	{
//		// you can log the base of your module here by using modInfo.lpBaseOfDll
//		// you can also log the size of your module by using modInfo.SizeOfImage
//
//		if (modInfo.SizeOfImage < MAXDWORD)
//		{
//			// where the pattern was found last
//			BYTE* pbyLastMatch;
//			// the offset from the module base to pbyLastMatch -> used as: module size - dwMatchOffset = range of bytes we still need to scan
//			DWORD dwMatchOffset = 0;
//			// are we done yet?
//			bool bMorphingFinished = false;
//			// good to know how many regions we morphed in total
//			DWORD dwRegionCount = 0;
//
//			while (!bMorphingFinished)
//			{
//				// search for the marker we will place in our code later on
//				pbyLastMatch = (BYTE*)FindPattern("triggerBot.dll", MARKER_BYTES, MARKER_MASK);
//
//				// if marker found
//				if (pbyLastMatch != NULL)
//				{
//					// call morphMemory which generates the junk code and writes it to memory
//					morphMemory(pbyLastMatch, MARKER_SIZE);
//					dwRegionCount++;
//
//					// this should be redundant if you think about it, better save than sorry though
//					pbyLastMatch++;
//					dwMatchOffset = (DWORD)pbyLastMatch - (DWORD)modInfo.lpBaseOfDll;
//				}
//				// if we found all markers
//				else
//				{
//					bMorphingFinished = true;
//					// here you can log how many regions got morphed by using dwRegionCount
//				}
//			}
//
//
//			// clean up
//
//		}
//	}
//	CloseHandle(hProcess);
//}

Vec2 crosshair;
D3DCOLOR color;

Vec2 entPosScreen;
Vec2 entHeadPosScreen;
Vec2 tl, tr;
Vec2 bl, br;
Vec2 htl, htr;
Vec2 hbl, hbr;
Vec2 l, r, t, b;
Vector3 headBone;
// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
		pDevice = o_pDevice;

	
	for (auto e : entList->entList) {
		if (e.entPtr && e.entPtr->hp > 0) {
		
			if (WorldToScreen(e.entPtr->pos, entPosScreen, viewMathrix)) {
				headBone = getBoneVec3(e.entPtr, 8);
			//	std::cout << 1;
				if (WorldToScreen(headBone, entHeadPosScreen, viewMathrix)) {
					if (e.entPtr != localPlayer && !e.entPtr->bDormant) {
						if (e.entPtr->teamNum == localPlayer->teamNum) {
							color = D3DCOLOR_ARGB(255, 0, 255, 0);
						}
						else {
							color = D3DCOLOR_ARGB(255, 255, 0, 0);
						}
					//	std::cout << 1;
						tl = tr = entHeadPosScreen;
						bl = br = entPosScreen;

						float height = entHeadPosScreen.y - entPosScreen.y;
						height = height > 0 ? height : -height;
						tl.x -= height / 4;
						tr.x += height / 4;

						bl.x -= height / 4;
						br.x += height / 4;

						DrawLine(tl.x, tl.y, bl.x, bl.y, 2, color);
						DrawLine(bl.x, bl.y, br.x, br.y, 2, color);
						DrawLine(br.x, br.y, tr.x, tr.y, 2, color);
						DrawLine(tr.x, tr.y, tl.x, tl.y, 2, color);



						if (e.entPtr->teamNum != localPlayer->teamNum) {
							float hp = e.entPtr->hp / 100.0f;
							htr = htl = tl;

							hbr = hbl = bl;


							htl.x -= height / 16;
							hbl.x -= height / 16;

							htl.y += height - height * hp;
							htr.y += height - height * hp;

							DrawLine(htl.x, htl.y, hbl.x, hbl.y, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
							DrawLine(hbl.x, hbl.y, hbr.x, hbr.y, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
							DrawLine(hbr.x, hbr.y, htr.x, htr.y, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
							DrawLine(htr.x, htr.y, htl.x, htl.y, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
						}

					}

					//DrawLine(entPosScreen.x, entPosScreen.y , windowWidth/2 , windowHeight , 2 ,color);
				}

			}
		}
	}

	int crosshairSize = 4;
	l = r = t = b = crosshair;
	l.x -= crosshairSize;
	r.x += crosshairSize;
	t.y -= crosshairSize;
	b.y += crosshairSize;
	// crosshair
	//DrawFilledRect(windowWidth / 2 - 2, windowHeight / 2 - 2, 4, 4, D3DCOLOR_ARGB(255, 255, 255, 255));
	DrawLine(l.x, l.y, r.x, r.y, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
	DrawLine(t.x, t.y, b.x, b.y, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
	// call og function
	oEndScene(pDevice);
}

void drawBone(Vector3 bone1, Vector3 bone2,  float* viewMatrix , float wight) {
	Vec2 bone1in2;
	Vec2 bone2in2;
	if (WorldToScreen(bone1, bone1in2, viewMathrix) && WorldToScreen(bone2, bone2in2, viewMathrix)) {
		DrawLine(bone1in2.x, bone1in2.y, bone2in2.x, bone2in2.y, wight, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}
void APIENTRY boneEsp(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
		pDevice = o_pDevice;

	for (auto e : entList->entList) {
		if (e.entPtr && e.entPtr->hp > 0 && e.entPtr != localPlayer) {
			Vector3 bone3 = getBoneVec3(e.entPtr, 3);
			Vector3 bone7 = getBoneVec3(e.entPtr, 7);
			Vector3 bone10 = getBoneVec3(e.entPtr, 10);
			Vector3 bone11 = getBoneVec3(e.entPtr, 11);
			Vector3 bone12 = getBoneVec3(e.entPtr, 12);
			Vector3 bone13 = getBoneVec3(e.entPtr, 13);
			Vector3 bone40 = getBoneVec3(e.entPtr, 40);
			Vector3 bone41 = getBoneVec3(e.entPtr, 41);
			Vector3 bone42 = getBoneVec3(e.entPtr, 42);
			Vector3 bone43 = getBoneVec3(e.entPtr, 43);
			Vector3 bone70 = getBoneVec3(e.entPtr, 70);
			Vector3 bone71 = getBoneVec3(e.entPtr, 71);
			Vector3 bone72 = getBoneVec3(e.entPtr, 72);
			Vector3 bone77 = getBoneVec3(e.entPtr, 77);
			Vector3 bone78 = getBoneVec3(e.entPtr, 78);
			Vector3 bone79 = getBoneVec3(e.entPtr, 79);

		
			drawBone(bone3, bone7,  viewMathrix ,4);
			drawBone(bone10, bone11,  viewMathrix,4);
			drawBone(bone11, bone12,  viewMathrix,2);
			drawBone(bone12, bone13, viewMathrix,2);
			drawBone(bone40, bone41,  viewMathrix,2);
			drawBone(bone41, bone42,  viewMathrix,2);
			drawBone(bone42, bone43, viewMathrix,2);
			drawBone(bone70, bone71, viewMathrix,3);
			drawBone(bone71, bone72, viewMathrix,3);
			drawBone(bone77, bone78, viewMathrix,3);
			drawBone(bone78, bone79, viewMathrix,3);
			drawBone(bone3, bone77, viewMathrix,3);
			drawBone(bone3, bone70, viewMathrix,3);
			drawBone(bone7, bone10, viewMathrix,2);
			drawBone(bone7, bone40, viewMathrix,2);
			
			
		}
	}
	oEndScene(pDevice);
}

void showMenu();

bool bTrigger = false;
bool bBoneEsp = false;
bool bAim = false;
bool bLiteAim = false;
bool bRadar = false;
bool bKnife = false;
bool bStand = false;
bool bZeus = false;
bool bHop = false;
bool bFlash = false;
bool bEsp = false;

void execute() {
	localPlayer = *(Ent**)(moduleBase + offsets::dwLocalPlayer);
	//std::cout << localPlayer << std::endl;
	if (localPlayer) {
		entList = (CBaseEntList*)(moduleBase + offsets::dwEntityList);
		aimBot.setEntList(entList);
		aimBot.setLocalPlayer(localPlayer);
		triggerBot.setLocalPlayer(localPlayer);
		triggerBot.setEntList(entList);
		/*if (GetAsyncKeyState(VK_NUMPAD1) & 1) {

			bRadar = !bRadar;
			showMenu();

		}*/
		/*if (GetAsyncKeyState(VK_NUMPAD2) & 1) {

			bStand = !bStand;
			forward->isMoving = 0;
			backward->isMoving = 0;
			right->isMoving = 0;
			left->isMoving = 0;
			showMenu();

		}*/
		if (GetAsyncKeyState(VK_NUMPAD3) & 1) {

			bZeus = !bZeus;
			showMenu();

		}
		if (GetAsyncKeyState(VK_NUMPAD4) & 1) {

			bHop = !bHop;
			showMenu();

		}
		/*if (GetAsyncKeyState(VK_NUMPAD5) & 1) {

			bFlash = !bFlash;
			showMenu();

		}
		if (GetAsyncKeyState(VK_NUMPAD6) & 1) {

			bTrigger = !bTrigger;
			showMenu();

		}
		if (GetAsyncKeyState(VK_NUMPAD7) & 1) {

			bAim = !bAim;
			showMenu();

		}
		if (GetAsyncKeyState(VK_NUMPAD8) & 1) {

			bBoneEsp = !bBoneEsp;
			if (bBoneEsp && !bEsp) {
				if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
					memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

					oEndScene = (tEndScene)drawHook::TrampHook((char*)d3d9Device[42], (char*)boneEsp, 7);
				}
			}
			else {
				drawHook::Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);
			}
			showMenu();

		}
		if (GetAsyncKeyState(VK_NUMPAD9) & 1) {

			bEsp = !bEsp;
			if (bEsp && !bBoneEsp) {
				if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
					memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
				
					oEndScene = (tEndScene)drawHook::TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
				}
			}
			else {
				drawHook::Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);
			}
			showMenu();

		}*/


		if (bRadar) {
			for (auto e : entList->entList) {
				std::cout << e.entPtr->hp << std::endl;
				if (e.entPtr) {
					if (e.entPtr != localPlayer && e.entPtr->teamNum != localPlayer->teamNum && e.entPtr->hp > 0) {
						e.entPtr->bSpotted = true;
					}
				}
			}
		}
		if (bTrigger) {
			Ent* entity = getEntById(entList, localPlayer->crosshairId - 1);
			if (localPlayer->crosshairId > 0 && entity && entity->teamNum != localPlayer->teamNum) {
				int weaponId = triggerBot.getWeaponID();
				//std::cout << weaponId << "\n";
				if (triggerBot.isShootWeapon(weaponId)) {
					//float distance = triggerBot.getDistance(entity);
					//int delay = triggerBot.getDelay(distance, weaponId);
					if (triggerBot.withScope(weaponId)) {
						if (localPlayer->isScoped) {
							triggerBot.shootLBA(370);
						}
					}
					else {
						triggerBot.shootLBA(230);
					}
				}
			}
		}
		if (bZeus) {
			Ent* entity = getEntById(entList, localPlayer->crosshairId - 1);

			if (localPlayer->crosshairId > 0 && entity && entity->teamNum != localPlayer->teamNum) {
				int weaponId = triggerBot.getWeaponID();
				//std::cout << localPlayer->entIdInCross << "\n";
				if (!triggerBot.isGrenade(weaponId)) {
					if (weaponId == triggerBot.WEAPON_TASER) {
						float distance = triggerBot.getDistance(entity);
						if (distance < 3.8) {
							//aimBot.aimAt(getBoneVec3(entity, 8));
							triggerBot.shootTaiserLBA();
						}
					}

				}
			}
		}
		//if (bKnife) {
		//	Ent* entity = getEntById(entList, localPlayer->crosshairId - 1);
		//	if (localPlayer->crosshairId > 0 && entity && entity->teamNum != localPlayer->teamNum) {
		//		int weaponId = triggerBot.getWeaponID();
		//		//	std::cout << localPlayer->crosshairId << "\n";
		//		if (!triggerBot.isGrenade(weaponId)) {
		//			if (weaponId == triggerBot.WEAPON_KNIFE || weaponId == triggerBot.WEAPON_KNIFE_T || weaponId == triggerBot.WEAPON_KNIFEGG) {
		//				float distance = triggerBot.getDistance(entity);
		//				if (distance < 1.60) {
		//					triggerBot.shootKnifeRBA();
		//				}
		//			}
		//		}
		//	}
		//}

		//if (bStand) {
		//	//Ent* entity = aimBot.getClosestEnemy();
		//	float closestDistance = 100000;
		//	Ent* closestEnemy = nullptr;
		//	for (auto e : entList->entList) {
		//		Ent* entity = e.entPtr;
		//		if (entity) {
		//			if (entity->teamNum != localPlayer->teamNum && entity->hp > 0) {
		//				float distance = sqrt(entity->pos.x * entity->pos.x + entity->pos.y * entity->pos.y + entity->pos.z * entity->pos.z);
		//				if (distance < closestDistance) {
		//					closestDistance = distance;
		//					closestEnemy = entity;
		//				}
		//			}
		//		}
		//	}
		//	Ent* entity = closestEnemy;
		//	if (entity) {
		//		//std::cout << entity->velocity.x << " " << entity->velocity.y << "\n";
		//		//std::cout << entity->angle.x << " " << entity->angle.y << "\n";
		//		/*Vector3 aim = getBoneVec3(entity, 8);
		//		double PI = 3.14159265358;
		//		Vector3 pos = getBoneVec3(localPlayer, 8);
		//		Vector3 deltaVec = { aim.x - pos.x , aim.y - pos.y ,aim.z - pos.z };
		//		float deltaVecLeght = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);
		//		float pitch = -asin(deltaVec.z / deltaVecLeght) * (180 / PI);
		//		float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);*/
		//		//	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180) {
		//	//	extraLocalPlayer->viewAngle.x = entity->angle.x;
		//	//	extraLocalPlayer->viewAngle.y = entity->angle.y;
		//		//}
		//		std::cout << localPlayer->angle.y << "\n";
		//		if (entity->angle.y > -180 && entity->angle.y <= -90) {
		//			if (entity->velocity.x < 0 && entity->velocity.y < 0) {
		//				forward->isMoving = 1;
		//			}
		//			else {
		//				forward->isMoving = 0;
		//			}
		//			if (entity->velocity.x > 0 && entity->velocity.y > 0) {
		//				backward->isMoving = 1;
		//			}
		//			else {
		//				backward->isMoving = 0;
		//			}
		//			if (entity->velocity.x > 0 && entity->velocity.y < 0) {
		//				left->isMoving = 1;
		//			}
		//			else {
		//				left->isMoving = 0;
		//			}
		//			if (entity->velocity.x < 0 && entity->velocity.y > 0) {
		//				right->isMoving = 1;
		//			}
		//			else {
		//				right->isMoving = 0;
		//			}
		//		}
		//		
		//		if (entity->angle.y > -90 && entity->angle.y <= 0) {
		//			if (entity->velocity.x > 0 && entity->velocity.y < 0) {
		//				forward->isMoving = 1;
		//			}
		//			else {
		//				forward->isMoving = 0;
		//			}
		//			if (entity->velocity.x < 0 && entity->velocity.y > 0) {
		//				backward->isMoving = 1;
		//			}
		//			else {
		//				backward->isMoving = 0;
		//			}
		//			if (entity->velocity.x > 0 && entity->velocity.y > 0) {
		//				left->isMoving = 1;
		//			}
		//			else {
		//				left->isMoving = 0;
		//			}
		//			if (entity->velocity.x < 0 && entity->velocity.y < 0) {
		//				right->isMoving = 1;
		//			}
		//			else {
		//				right->isMoving = 0;
		//			}
		//		}
		//		if (entity->angle.y > 0 && entity->angle.y <= 90) {
		//			if (entity->velocity.x > 0 && entity->velocity.y > 0) {
		//				forward->isMoving = 1;
		//			}
		//			else {
		//				forward->isMoving = 0;
		//			}
		//			if (entity->velocity.x < 0 && entity->velocity.y < 0) {
		//				backward->isMoving = 1;
		//			}
		//			else {
		//				backward->isMoving = 0;
		//			}
		//			if (entity->velocity.x < 0 && entity->velocity.y > 0) {
		//				left->isMoving = 1;
		//			}
		//			else {
		//				left->isMoving = 0;
		//			}
		//			if (entity->velocity.x > 0 && entity->velocity.y < 0) {
		//				right->isMoving = 1;
		//			}
		//			else {
		//				right->isMoving = 0;
		//			}
		//		}
		//		if (entity->angle.y > 90 && entity->angle.y <= 180) {
		//			if (entity->velocity.x < 0 && entity->velocity.y > 0) {
		//				forward->isMoving = 1;
		//			}
		//			else {
		//				forward->isMoving = 0;
		//			}
		//			if (entity->velocity.x > 0 && entity->velocity.y < 0) {
		//				backward->isMoving = 1;
		//			}
		//			else {
		//				backward->isMoving = 0;
		//			}
		//			if (entity->velocity.x < 0 && entity->velocity.y < 0) {
		//				left->isMoving = 1;
		//			}
		//			else {
		//				left->isMoving = 0;
		//			}
		//			if (entity->velocity.x > 0 && entity->velocity.y > 0) {
		//				right->isMoving = 1;
		//			}
		//			else {
		//				right->isMoving = 0;
		//			}
		//		}
		//		
		//	}
		//}
		if (bHop) {
			if (GetAsyncKeyState(VK_SPACE)) {

				if (isPlayerMoving(localPlayer)) {
					if (localPlayer->actionFlag == 257) {//257 = on land 
						jump->jump = 6;
					}
				}

			}
		}
		if (bFlash) {

			if (localPlayer->flashDur > 0) {
				localPlayer->flashDur = 0;
			}
		}
		if (bAim) {


			//if (GetAsyncKeyState(VK_LSHIFT) & 1) {
			Ent* enemy = aimBot.getClosestEnemy(aimBot.isVisible(EngineTrace));

			if (enemy && !enemy->bDormant) {
				//	std::cout << count << std::endl;
				aimBot.aimAt(getBoneVec3(enemy, count));
				//	count++;
			//	}
			}
		}

		/*if (bLiteAim) {
			if (GetAsyncKeyState(VK_LBUTTON)) {


				Ent* enemy = aimBot.getClosestEnemy(aimBot.isVisible(EngineTrace));

				if (enemy && !enemy->bDormant) {
					aimBot.aimAt(getBoneVec3(enemy, 8), aimBot.getDistance(enemy->pos));
				}
			}
		}*/

	}
}

DWORD dwJMPback;
__declspec(naked) void myFunc() {


	execute();

	__asm cmp byte ptr ds : [esi + 0x939] , 0
	__asm JMP[dwJMPback]


}
DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	std::cout << "here1\n";
	dwJMPback = (DWORD)(moduleBase + 0x3119F2);
	int lenght = 7;
	showMenu();
	//morphModule();
	std::cout << "here2\n";
	Hook hook = Hook((void*)dwJMPback, myFunc, lenght);

	hook.EnableMiddle();
	dwJMPback += lenght;
	std::cout << "here3\n";
	while (1) {
		Vector3 punchAngle = localPlayer->punchAngle;
		crosshair.x = windowWidth / 2 - (windowWidth / 90 * punchAngle.y);
		crosshair.y = windowHeight / 2 + (windowHeight / 90 * punchAngle.x);
		//std::cout << crosshair.x << " " << crosshair.y << "\n";
		if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
			break;
		}
	}
	if (bEsp || bBoneEsp) {
		drawHook::Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);
	}
	hook.DisableMiddle();

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);


	return 0;
}


void showMenu() {

	std::cout << "Radar numpad1 - ";
	if (bRadar) {
		std::cout << " on\n";
	}
	else {
		std::cout << " off\n";
	}
	/*std::cout << "Stand numpad2 - ";
	if (bStand) {
		std::cout << " on\n";
	}
	else {
		std::cout << " off\n";
	}*/

	std::cout << "Auto Zeus numpad3 - ";
	if (bZeus) {
		std::cout << " on\n";
	}
	else {
		std::cout << " off\n";
	}

	std::cout << "bHop numpad4 - ";
	if (bHop) {
		std::cout << " on\n";
	}
	else {
		std::cout << " off\n";
	}

	std::cout << "AntiFlash numpad5 - ";
	if (bFlash) {
		std::cout << " on\n";
	}
	else {
		std::cout << " off\n";
	}

	std::cout << "Trigger numpad6 - ";
	if (bTrigger) {
		std::cout << " on\n";
	}
	else {
		std::cout << " off\n";
	}

	std::cout << "Aim numpad7 - ";
	if (bAim) {
		std::cout << " on\n";
	}
	else {
		std::cout << " off\n";
	}
	std::cout << "bBoneEsp numpad8 - ";
	if (bBoneEsp) {
		std::cout << " on\n";
	}
	else {
		std::cout << " off\n";
	}
	std::cout << "Esp numpad9 - ";
	if (bEsp) {
		std::cout << " on\n";
	}
	else {
		std::cout << " off\n";
	}
	std::cout << "Exit numpad0\n\n\n";

}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE hThread = nullptr;
		hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
		if (hThread) {
			CloseHandle(hThread);
		}

	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

