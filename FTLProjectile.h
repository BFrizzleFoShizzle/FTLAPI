#pragma once

#include <vector>
#include <chaiscript\chaiscript.hpp>
#include <Windows.h>

struct Projectile {
	// lazer = 0x0078FE48, beam = 0x0078FBC8, missile = 0x00793B88
	void* header; // + 0
	void* unknown1;// + 4
	char unknown2[8];// + 8
	float x;// + 10
	float y;// + 14
	// apparently, these are floats?
	float unknown3;// + 18
	float unknown4;// + 1C
	float unknown5;// + 20
	float unknown6;// + 24
	float unknown7;// + 28
	float angle;// + 2C
	char unknown8[4];// + 30
	// this appears to be some sort of index for identifying the projectile?
	int unknownIndex;// + 34
	int damage;// + 38
	int shieldPenetration;// + 3C
	// these are all out of ten...
	// code appears to do (rand()%10)<chance
	int fireChance;// + 40
	int breachChance;// + 44
	int stunChance;// + 48
	int ionDamage;// + 4C
	char unknown9[4];// + 50
	int crewDamage;// + 54
	char unknown10[40];// + 58
	// incorrect?
	//void* target;// + 98

};

class ProjectileWrapper {
public:
	ProjectileWrapper(Projectile* representedProj);
	ProjectileWrapper(void);
	float getX(void);
	float getY(void);
	void setX(float x);
	void setY(float y);
	float getAngle(void);
	void setAngle(float angle);
	int getDamage(void);
	void setDamage(int damage);
	int getShieldPenetration(void);
	void setShieldPenetration(int penetration);
	int getFireChance(void);
	void setFireChance(int chance);
	int getBreachChance(void);
	void setBreachChance(int chance);
	int getStunChance(void);
	void setStunChance(int chance);
	int getIonDamage(void);
	void setIonDamage(int damage);
	int getCrewDamage(void);
	void setCrewDamage(int damage);
	bool isActive(void);

	void setProjectilePointer(Projectile* representedShip);
	Projectile* getProjectilePointer(void);
private:
	Projectile* pProj;
};


void SetupProjectileHooks(HANDLE process);

std::vector<chaiscript::Boxed_Value> GetActiveProjectiles();