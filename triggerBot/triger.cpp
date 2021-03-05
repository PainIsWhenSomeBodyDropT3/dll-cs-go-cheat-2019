#include "pch.h"
#include "triger.h"
#include <iostream>

void  trigger::setLocalPlayer(Ent* localPlayer) {
	this->localPlayer = localPlayer;
}
void  trigger::setEntList(CBaseEntList* entList) {
	this->entList = entList;
}
void trigger::shootLBA(int delay)
{
	lba->attack = 6;
	//Sleep(delay);
	
}

void  trigger::shootKnifeRBA() {
	rba->attack=6;
	//Sleep(50);
	
}

void  trigger::shootTaiserLBA() {
	
	lba->attack = 6;
	//Sleep(5);
//	lba->attack = 4;
}

float trigger::getDistance(Ent* entity) {
	Vector3 playerVec = localPlayer->pos;
	Vector3 entityVec = entity->pos;

	return sqrt(pow(playerVec.x - entityVec.x, 2) + pow(playerVec.y - entityVec.y, 2) + pow(playerVec.z - entityVec.z, 2)) * 0.0254;
}

int trigger::getDelay(float distance, int weaponId) {
	int delay=0;
	switch (weaponId) {

	case WEAPON_DEAGLE: {delay =0; break; }
	case WEAPON_ELITE: {delay = 0; break; }
	case WEAPON_FIVESEVEN: {delay = 0; break; }
	case WEAPON_GLOCK: {delay = 0; break; }
	case WEAPON_AK47: {delay = 0; break; }
	case WEAPON_AUG: {delay = 0; break; }
	case WEAPON_AWP: {delay = 0; break; }
	case WEAPON_FAMAS: {delay = 0; break; }
	case WEAPON_G3SG1: {delay = 0; break; }
	case WEAPON_GALILAR: {delay = 0; break; }
	case WEAPON_M249: {delay = 0; break; }
	case WEAPON_M4A1: {delay = 0; break; }
	case WEAPON_MAC10: {delay = 0; break; }
	case WEAPON_P90: {delay = 0; break; }
	case WEAPON_MP5: {delay = 0; break; }
	case WEAPON_UMP45: {delay = 0; break; }
	case WEAPON_XM1014: {delay = 0; break; }
	case WEAPON_BIZON: {delay = 0; break; }
	case WEAPON_MAG7: {delay = 0; break; }
	case WEAPON_NEGEV: {delay = 0; break; }
	case WEAPON_SAWEDOFF: {delay = 0; break; }
	case WEAPON_TEC9: {delay = 0; break; }
	case WEAPON_HKP2000: {delay = 0; break; }
	case WEAPON_MP7: {delay = 0; break; }
	case WEAPON_MP9: {delay = 0; break; }
	case WEAPON_NOVA: {delay = 0; break; }
	case WEAPON_P250: {delay = 0; break; }
	case WEAPON_SCAR20: {delay = 0; break; }
	case WEAPON_SG556: {delay = 0; break; }
	case WEAPON_SSG08: {delay = 0; break; }
	case WEAPON_M4A1_SILENCER: {delay = 0; break; }
	case WEAPON_USP_SILENCER: {delay = 0; break; }
	case WEAPON_CZ75A: {delay = 0; break; }
	case WEAPON_REVOLVER: {delay = 0; break; }
	}
	
	return delay * distance;
}

int trigger::getWeaponID() {
	int currentWeaponSerialNumber = localPlayer->activeWeapon;

	int weaponEntity = *(int*)((int)entList + ((currentWeaponSerialNumber & 0xFFF) - 1) * 0x10);
	if (weaponEntity != NULL) {
		return *(int*)(weaponEntity + extraOffset.plusToWeapon);
	}

}
bool trigger::isShootWeapon(int weaponId) {
	switch (weaponId) {
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_AWP:
	case WEAPON_FAMAS:
	case WEAPON_G3SG1:
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_MP5:
	case WEAPON_UMP45:
	case WEAPON_XM1014:
	case WEAPON_BIZON:
	case WEAPON_MAG7:
	case WEAPON_NEGEV:
	case WEAPON_SAWEDOFF:
	case WEAPON_TEC9:
	case WEAPON_HKP2000:
	case WEAPON_MP7:
	case WEAPON_MP9:
	case WEAPON_NOVA:
	case WEAPON_P250:
	case WEAPON_SCAR20:
	case WEAPON_SG556:
	case WEAPON_SSG08:
	case WEAPON_M4A1_SILENCER:
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER: 
	case 262205: {
		return true;
	}
	}
	return false;
}

bool trigger::isGrenade(int weaponId) {
	switch (weaponId) {
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_DECOY: {
		return true;
	}
	}
	return false;
}

bool trigger::withScope(int weaponId) {
	switch (weaponId) {
	
	case WEAPON_AWP:
	case WEAPON_SG556:
	case WEAPON_SCAR20:
	case WEAPON_G3SG1:
	case WEAPON_SSG08: {
		return true;
	}
	}
	return false;
}