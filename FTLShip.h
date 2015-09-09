#pragma once
#include "FTLWeapon.h"

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
	char unknown2;
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
	//skip the rest for now...
};

extern ship* ships;
extern ship* playerShip;

ship* getPlayerShip(void);

ship* getEnemyShip(void);

class ShipWrapper {
public:
	ShipWrapper(ship* representedShip);
	ShipWrapper(void);
	int getHealth(void);
	void setHealth(int);
	void setShipPointer(ship* representedShip);
	ship* getShipPointer(void);
private:
	ship *pShip;
};