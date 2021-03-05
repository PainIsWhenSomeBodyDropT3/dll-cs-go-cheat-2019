#pragma once
#include "pch.h"
#include "Vector3.h"
#include "csgoVector.h"
#include "includes.h"
// Created with ReClass.NET by KN4CK3R

class Vector3;

struct Matrix3x4 {
	float
		x1, x2, x3, x4,
		y1, y2, y3, y4,
		z1, z2, z3, z4;
};
	


// Created with ReClass.NET by KN4CK3R

class BoneMatrix
{
public:
	Matrix3x4 bone; //0x0000
}; //Size: 0x0030

class BoneMatrixs
{
public:
	BoneMatrix boneMatrixList[94]; //0x0000
}; //Size: 0x11A0

class Ent//22
{
public:
	char pad_0000[237]; //0x0000
	bool bDormant; //0x00ED
	char pad_00EE[6]; //0x00EE
	int32_t teamNum; //0x00F4
	char pad_00F8[8]; //0x00F8
	int32_t hp; //0x0100
	int32_t actionFlag; //0x0104
	char pad_0108[12]; //0x0108
	Vector3 velocity; //0x0114
	char pad_0120[12]; //0x0120
	Vector3 angle; //0x012C
	Vector3 pos; //0x0138
	char pad_0144[2041]; //0x0144
	bool bSpotted; //0x093D
	char pad_093E[7530]; //0x093E
	class BoneMatrixs* ptrBoneMatrixs; //0x26A8
	char pad_26AC[2124]; //0x26AC
	int32_t activeWeapon; //0x2EF8
	char pad_2EFC[304]; //0x2EFC
	Vector3 punchAngle; //0x302C
	char pad_3038[2288]; //0x3038
	bool isScoped; //0x3928
	char pad_3929[27383]; //0x3929
	float flashDur; //0xA420
	char pad_A424[4032]; //0xA424
	int32_t crosshairId; //0xB3E4
	char pad_B3E8[644]; //0xB3E8
}; //Size

class ClassInfo
{
public:
	class Ent* entPtr; //0x0000
	int32_t someInt; //0x0004
	class ClassInfo* blink; //0x0008
	class ClassInfo* flink; //0x000C
}; //Size: 0x0010

class CBaseEntList
{
public:
	ClassInfo entList[64]; //0x0000
}; //Size: 0x0400

class ExtraEnt
{
public:
	char pad_0000[19848]; //0x0000
	Vector3 viewAngle; //0x4D88
	char pad_4D94[60]; //0x4D94
}; //Size: 0x4DD0

struct Vec4 {
	float x, y, z,w;
};
struct Vec3 {
	float x, y, z;
};
struct Vec2 {
	float x, y;
	Vec2() {}
};

class Forward
{
public:
	bool isMoving; //0x0000

}; //Size: 0x0840


class Backward
{
public:
	bool isMoving; //0x0000
	
}; //Size: 0x0040


class Left
{
public:
	bool isMoving; //0x0000
	
}; //Size: 0x0040


class Right
{
public:
	bool isMoving; //0x0000
	
}; //Size: 0x0040

Ent* getEntById(CBaseEntList* list, int id);
bool isPlayerMoving(Ent* ent);
Vector3 getBoneVec3(Ent* ent, int boneId);
vec3 getBoneVec3csgo(Ent* ent, int boneId);
bool WorldToScreen(Vector3 pos, Vec2& screen , float* );
	

