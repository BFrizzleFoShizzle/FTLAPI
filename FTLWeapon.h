#pragma once
struct target { 
	float x;
	float y;
};

struct weapon {
	char unknown1[8];
	float chargeTime;
	float chargeFinsh;
	char unknown2[0xA4];
	char autoFire; // 0 = disabled, 1 = enabled
	char targeting; // 0 = no target, 1 = target
	char powered; // 0 = unpowered not charging, 1 = powered charging
	char unknown3[5];
	target* targetArr;
	char unknown4[0x1C];
	char* weaponName;
	int numShots;
	//rest doesn't matter
};
