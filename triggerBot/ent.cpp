#include "pch.h"
#include "ent.h"
Ent* getEntById(CBaseEntList* list, int id) {
	int i = 0;
	for (auto e : list->entList) {
		if (i == id) {
			return e.entPtr;
		}
		i++;
	}
	return NULL;
}
bool isPlayerMoving(Ent* ent) {
	Vector3 velocity = ent->velocity;
	return velocity.x + velocity.y + velocity.z != 0;
}
Vector3 getBoneVec3(Ent* ent,int boneId)
{
	Vector3 bonePos;
	int counter = 0;
	for (BoneMatrix b : ent->ptrBoneMatrixs->boneMatrixList) {
		if (counter == boneId) {
			bonePos.x = b.bone.x4;
			bonePos.y = b.bone.y4;
			bonePos.z = b.bone.z4;
			break;
		}
		counter++;
	}
	return bonePos;
}
vec3 getBoneVec3csgo(Ent* ent, int boneId) {
	vec3 bonePos;
	int counter = 0;
	for (BoneMatrix b : ent->ptrBoneMatrixs->boneMatrixList) {
		if (counter == boneId) {
			bonePos.x = b.bone.x4;
			bonePos.y = b.bone.y4;
			bonePos.z = b.bone.z4;
			break;
		}
		counter++;
	}
	return bonePos;
}
bool WorldToScreen(Vector3 pos, Vec2& screen , float *viewMatrix) {
	Vec4 clipCoords;
	
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}