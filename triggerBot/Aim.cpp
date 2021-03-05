#include "pch.h"
#include "Aim.h"
#include "csgo.h"
#include "csgotrace.h"
#include "csgoVector.h"


std::vector<Ent*> Aim::isVisible(IEngineTrace* EngineTrace) {

	std::vector<Ent*> visibleEnts;
	for (auto e : baseEntList->entList) {
		Ent* entity = e.entPtr;
		if (entity) {
			if (entity->teamNum != localPlayer->teamNum && entity->hp > 0) {

				vec3 eyepos = getBoneVec3csgo(localPlayer, 8);
				vec3 targeteyepos = getBoneVec3csgo(entity, 8);

				CGameTrace trace;
				Ray_t ray;
				CTraceFilter tracefilter;
				tracefilter.pSkip = (void*)localPlayer;

				ray.Init(eyepos, targeteyepos);

				EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &tracefilter, &trace);
				
			
				if (entity == trace.hit_entity)
				{
					
					visibleEnts.push_back(entity);
				}
			}
		}
	}
	return visibleEnts;
}

void  Aim::setLocalPlayer(Ent* localPlayer) {
	this->localPlayer = localPlayer;
}
void  Aim::setEntList(CBaseEntList* baseEntList) {
	this->baseEntList = baseEntList;
}
Ent* Aim::getClosestEnemy(std::vector<Ent*> visibleEnts) {
	float closestDistance = 100000;
	Ent* closestEnemy = nullptr;
	for (auto e : visibleEnts) {
		Ent* entity = e;
		if (entity) {
			if (entity->teamNum != localPlayer->teamNum && entity->hp > 0) {
				float distance = getDistance(entity->pos);
				if (distance < closestDistance) {
					closestDistance = distance;
					closestEnemy = entity;
				}
			}
		}
	}
	return closestEnemy;
}
Ent* Aim::getClosestEnemy() {
	float closestDistance = 100000;
	Ent* closestEnemy = nullptr;
	for (auto e : baseEntList->entList) {
		Ent* entity = e.entPtr;
		if (entity) {
			if (entity->teamNum != localPlayer->teamNum && entity->hp > 0) {
				float distance = getDistance(entity->pos);
				if (distance < closestDistance) {
					closestDistance = distance;
					closestEnemy = entity;
				}
			}
		}
	}
	return closestEnemy;
}
float Aim::getDistance(Vector3 enemyVec) {
	Vector3 playerVec = localPlayer->pos;
	return sqrt(pow(playerVec.x - enemyVec.x, 2) + pow(playerVec.y - enemyVec.y, 2) + pow(playerVec.z - enemyVec.z, 2));
}
float Aim::getLenght(Vector3 vec1)
{
	return sqrt(vec1.x * vec1.x + vec1.y * vec1.y + vec1.z * vec1.z);

}

void Aim::aimAt(Vector3 aim, float dist) {

	double PI = 3.14159265358;


	Vector3 pos = getBoneVec3(localPlayer, 8);


	Vector3 deltaVec = { aim.x - pos.x , aim.y - pos.y ,aim.z - pos.z };
	float deltaVecLeght = getLenght(deltaVec);

	float pitch = -asin(deltaVec.z / deltaVecLeght) * (180 / PI);
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);
	float difPitch = pitch - extraLocalPlayer->viewAngle.x;
	float difYaw = yaw - extraLocalPlayer->viewAngle.y;
	dist -= (abs(difPitch) + abs(difYaw));
	pitch = extraLocalPlayer->viewAngle.x + difPitch/dist;
	yaw = extraLocalPlayer->viewAngle.y + difYaw/dist;
	

	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180) {

		extraLocalPlayer->viewAngle.x = pitch;
		extraLocalPlayer->viewAngle.y = yaw;
	}

}
void Aim::aimAt(Vector3 aim) {
	double PI = 3.14159265358;


	Vector3 pos = getBoneVec3(localPlayer, 8);


	Vector3 deltaVec = { aim.x - pos.x , aim.y - pos.y ,aim.z - pos.z };
	float deltaVecLeght = getLenght(deltaVec);

	float pitch = -asin(deltaVec.z / deltaVecLeght) * (180 / PI);
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);
	


	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180) {

		extraLocalPlayer->viewAngle.x = pitch;
		extraLocalPlayer->viewAngle.y = yaw;
	}
}