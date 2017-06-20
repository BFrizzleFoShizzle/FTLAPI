#pragma once
#include "FTLWeapon.h"
#include <string>
#include <memory>

struct Unit {
	char unknown1[0xC];
	float x; //current x
	float y; //current y
	char unknown2[8];
	float xDirectGoal; //value FROM pathfinding code, within the current room
	float yDirectGoal; //value FROM pathfinding code, within the current room
	char unknown3[8];
	float health;
	float maxHealth; //NOT master
	char unknown4[0x2c];
	float xPathfindedGoal; // pathfinding end destination, anywhere
	float yPathfindedGoal; // pathfinding end destination, anywhere
	char unknown5[0x20];
	char selection; //0 = nothing, 1 = selected, 2 = hover
	char unknown6[0x17f];
	char* name;
	char* name2; // ...
};

struct UtilityManager {
	char unknown1[0x34];
	char operated; //0 if not operated, 1 if operated
	char unknown2[4];
	int powerLevel;
	char unknown3[0x160];
	weapon** weapons; //(weapons[0]) = pointer to first weapon
	weapon** weaponsEnd; //pointer to ONE PAST the last element of weapons array
	//rest doesn't matter
};



struct Ship {
	char unknown1[0x24];
	UtilityManager* oxygenManager;
	char unknown2[0x7C];
	UtilityManager* shieldManager;
	UtilityManager* weaponManager;
	UtilityManager* droneManager; //haven't reversed the drone manager or drones yet
	UtilityManager* engineManager;
	UtilityManager* medicalManager;
	char unknown3[0xC];
	Unit** units; //(units[0]) = pointer to first unit on ship
	Unit** unitsEnd; //pointer to ONE PAST the last element of units array
	char unknown4[0x60];
	int health;
	int maxHealth; // 0x130 | 0x3C
	char unknown5[0x2C];
	char* shipImage;
	char unknown6[0x24];
	char* cloakImage; // 188
	char unknown7[0x27C];
	Ship* enemyShip; // 0x404 | 0x310
	char unknown8[0x44];
	int scrap; // 0x450 | 0x35C
	char unknown9[0x64];
	char* id;// 4B8
	char* name;
	char* displayName;
	//skip the rest for now...
};

extern Ship* playerShip;
extern Ship* npcShip;

Ship* GetPlayerShip(void);

Ship* GetNPCShip(void);

class UtilityManagerWrapper {
public:
	UtilityManagerWrapper(UtilityManager* representedUtil);
	bool getOperated(void);
	void setOperated(bool);
	int getPowerLevel(void);
	void setPowerLevel(int power);
	void setUtilityManagerPointer(UtilityManager* representedUtil);
	UtilityManager* getUtilityManagerPointer(void);
private:
	UtilityManager *pUtil;
};

class ShipWrapper {
public:
	ShipWrapper(Ship* representedShip);
	ShipWrapper(void);
	int getHealth(void);
	void setHealth(int);
	std::string getShipImageName(void);
	std::string getCloakImageName(void);
	std::string getID(void);
	std::string getName(void);
	std::string getDisplayName(void);
	std::shared_ptr<UtilityManagerWrapper> getOxygenManager(void);
	std::shared_ptr<UtilityManagerWrapper> getWeaponManager(void);
	std::shared_ptr<UtilityManagerWrapper> getDroneManager(void);
	std::shared_ptr<UtilityManagerWrapper> getEngineManager(void);
	std::shared_ptr<UtilityManagerWrapper> getMedicalManager(void);
	std::shared_ptr<UtilityManagerWrapper> getShieldManager(void);
	void setShipPointer(Ship* representedShip);
	Ship* getShipPointer(void);
private:
	Ship *pShip;
	std::shared_ptr<UtilityManagerWrapper> oxygenMan;
	std::shared_ptr<UtilityManagerWrapper> weaponMan;
	std::shared_ptr<UtilityManagerWrapper> droneMan;
	std::shared_ptr<UtilityManagerWrapper> engineMan;
	std::shared_ptr<UtilityManagerWrapper> medicalMan;
	std::shared_ptr<UtilityManagerWrapper> shieldMan;
};