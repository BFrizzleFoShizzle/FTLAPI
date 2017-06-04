#pragma once

//int playerShipsStack = 0x0028EF6C;
int playerShipsExe = 0x0079BE2C;

struct ShipDescriptor {
	// some metadata?
	char unknown1[4];
	ShipDescriptor* parent;
	ShipDescriptor* lessThan;
	ShipDescriptor* greaterThan;
	// may or may not be important...
	char* shipID2;
	// always  0x007923C8, technically a pointer
	// pointer to base is often used as the starting point for the ship descriptor struct, but isn't
	int base;
	char unknown2[0x1C];
	char* shipClass;
	char unknown3[4];
	char* shipID;
	char* shipName;
	// not 100% sure
	char* shipClassSlave;
	char* layoutName;
	char* hullImageName;
	char* cloakImageName;
	char* shieldsImageName;
	char* floorImageName;
	char unknown4[8];
	// room descriptor array start (pointer)
	int roomStart;
	// room descriptor array start (pointer)
	int roomEnd;
	// pointer, part way through the rooms list (between roomStart and roomEnd)
	int roomSomething;
	char unknown5[4];
	// pointer to list of starting systems, stored as ints with their type
	int* startingSystems;
	char unknown6[8];
	int maxDrones;
	char unknown7[0x28];
	int numWeaponMounts;
	char unknown8[4];
	// array of cstrings containing IDs of weapons, array lenght is numWeapon
	char** startingWeaponsStart;
	// pointer to space AFTER last weapon
	char** startingWeaponsEnd;
	// the same?
	char** startingWeaponsEnd2;
	int startingMissles;
	int startingDroneParts;
	int maxHP;
	// might be incorrect
	int numUnits;
	//array of cstrings containing unit type IDs
	char** unitTypesStart;
	// pointer to space AFTER last unit type
	char** unitTypesEnd;
	// the same?
	char** unitTypesEnd2;
	char unknown9[0xc];
	int startingShipPower;
	char unknown10[0x24];
};