#pragma once
#include "pch.h"
#include "csgotrace.h"
#include "csgoVector.h"
#include "ent.h"




class Aim
{
public:
	uintptr_t moduleBase;
	CBaseEntList* baseEntList;
	Ent* localPlayer;
	ExtraEnt* extraLocalPlayer;
	





	Aim(uintptr_t moduleBase, CBaseEntList* baseEntList, Ent* localPlayer , ExtraEnt* extraLocalPlayer) {
		this->moduleBase = moduleBase;
		this->baseEntList = baseEntList;
		this->localPlayer = localPlayer;
		this->extraLocalPlayer = extraLocalPlayer;
	}
	void setLocalPlayer(Ent* localPlayer);
		
	
	void setEntList(CBaseEntList* baseEntList);
		
	
	Ent* getClosestEnemy(std::vector<Ent*> visibleEnts);
	Ent* getClosestEnemy();
	float getDistance(Vector3 enemyVEc);
	float getLenght(Vector3 vec1);
	void aimAt(Vector3 aim, float dist);
	void aimAt(Vector3 aim);
	Vector3* GetOrigin();
	std::vector<Ent*> isVisible(IEngineTrace* EngineTrace);
};

