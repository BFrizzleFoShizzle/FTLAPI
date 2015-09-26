#pragma once
#include "FTLWeapon.h"
#include <string>
#include <memory>

struct unit {
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

struct utilityManager {
	char unknown1[0x34];
	char operated; //0 if not operated, 1 if operated
	char unknown2[4];
	int powerLevel;
	char unknown3[0x160];
	weapon** weapons; //(weapons[0]) = pointer to first weapon
	weapon** weaponsEnd; //pointer to ONE PAST the last element of weapons array
	//rest doesn't matter
};



struct ship {
	char unknown1[0x24];
	utilityManager* oxygenManager;
	char unknown2[0x7C];
	utilityManager* shieldManager;
	utilityManager* weaponManager;
	utilityManager* droneManager; //haven't reversed the drone manager or drones yet
	utilityManager* engineManager;
	utilityManager* medicalManager;
	char unknown3[0xC];
	unit** units; //(units[0]) = pointer to first unit on ship
	unit** unitsEnd; //pointer to ONE PAST the last element of units array
	char unknown4[0x60];
	int health;
	char unknown5[0x30];
	char* shipImage;
	char unknown6[0x24];
	char* cloakImage;
	char unknown7[0x32C];
	char* id;
	char* name;
	char* displayName;
	//skip the rest for now...
};

extern ship* ships;
extern ship* playerShip;

ship* getPlayerShip(void);

ship* getEnemyShip(void);

class UtilityManagerWrapper {
public:
	UtilityManagerWrapper(utilityManager* representedUtil);
	bool getOperated(void);
	void setOperated(bool);
	int getPowerLevel(void);
	void setPowerLevel(int power);
	void setUtilityManagerPointer(utilityManager* representedUtil);
	utilityManager* getUtilityManagerPointer(void);
private:
	utilityManager *pUtil;
};

class ShipWrapper {
public:
	ShipWrapper(ship* representedShip);
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
	void setShipPointer(ship* representedShip);
	ship* getShipPointer(void);
private:
	ship *pShip;
	std::shared_ptr<UtilityManagerWrapper> oxygenMan;
	std::shared_ptr<UtilityManagerWrapper> weaponMan;
	std::shared_ptr<UtilityManagerWrapper> droneMan;
	std::shared_ptr<UtilityManagerWrapper> engineMan;
	std::shared_ptr<UtilityManagerWrapper> medicalMan;
	std::shared_ptr<UtilityManagerWrapper> shieldMan;
};