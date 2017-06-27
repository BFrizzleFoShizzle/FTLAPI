#include "FTLProjectile.h"
#include "DirtyHooker.h"


std::vector<Projectile*> *activeProjectiles;

/* 
// outdated
__declspec(naked) void ProjectileCreateHook(void) 
{
	__asm 
	{
		push[esp + 0xE4];
		// do the call
		call ProcessProjectiles;
	}
}
*/
/*

1 proj (missile) post draw
FTLGame.exe+39BA94
+1F34
+0 (+4 is 4 higher, +8 is 20 higher)
+0 (pointer to created projectile, +4 +8, +C all contain previous pointer to projectile (all the same))
+0
*/

void __stdcall CreateDroneProjectileHook2(Projectile* proj)
{
	// todo
	proj->damage = 0;
}

__declspec(naked) void CreateDroneProjectileHook(void)
{
	__asm
	{
		push eax;
		call CreateDroneProjectileHook2;
		ret;
	}
}

void __stdcall CreateShipProjectileHook2(Projectile* proj)
{
	// todo
	proj->damage = 0;
}

__declspec(naked) void CreateShipProjectileHook(void)
{
	__asm
	{
		push eax;
		call CreateShipProjectileHook2;
		ret;
	}
}

void SetupProjectileHooks(HANDLE process) {
	// DOESN'T INCLUDE ASTROIDS or DRONE PROJECTILES
	//RET6CALL5AutoHookNaked(ProjectileCreateHook, 0x400000+0x3B3CC, 6, process);

	RET6CALL5AutoHookNaked(CreateShipProjectileHook, 0x400000 + 0x59F83, 6, process);
	RET6CALL5AutoHookNaked(CreateDroneProjectileHook, 0x400000 + 0x59D83, 6, process);

	/*
	// untested, alternative way of finding projectiles (and checking if an existing projectile is still "in-game")
	int* pointer = (int*)(0x400000 + 0x39BA94);
	if (pointer != nullptr)
		pointer = (int*)*pointer;
	if (pointer != nullptr)
		activeProjectiles = (std::vector<Projectile*> *)(*(pointer+0x7CD));
	*/
}