#include "FTLProjectile.h"
#include "DirtyHooker.h"


std::vector<Projectile*> **activeProjectiles;

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

__declspec(naked) void CreateProjectileVectorHook(void)
{
	__asm
	{
		add eax, 0x1f34;
		mov [activeProjectiles], eax;
		sub eax, 0x1f34;
		ret;
	}
}


void SetupProjectileHooks(HANDLE process) {
	activeProjectiles = nullptr;

	RET6CALL5AutoHookNaked(CreateShipProjectileHook, 0x400000 + 0x59F83, 6, process);
	RET6CALL5AutoHookNaked(CreateDroneProjectileHook, 0x400000 + 0x59D83, 6, process);
	RET6CALL5AutoHookNaked(CreateProjectileVectorHook, 0x400000 + 0x63159, 6, process);

	/*
	// untested, alternative way of finding projectiles (and checking if an existing projectile is still "in-game")
	int* pointer = (int*)(0x400000 + 0x39BA94);
	if (pointer != nullptr)
		pointer = (int*)*pointer;
	if (pointer != nullptr)
		activeProjectiles = (std::vector<Projectile*> *)(*(pointer+0x7CD));
	*/
}

// PROJECTILE WRAPPER

ProjectileWrapper::ProjectileWrapper(Projectile* representedProj)
{
	pProj = representedProj;
}

ProjectileWrapper::ProjectileWrapper(void)
{
	pProj = nullptr;
}

float ProjectileWrapper::getX(void)
{
	if (pProj && isActive())
	{
		return pProj->x;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

float ProjectileWrapper::getY(void)
{
	if (pProj && isActive())
	{
		return pProj->y;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

void ProjectileWrapper::setX(float x)
{
	if (pProj && isActive())
	{
		pProj->x = x;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

void ProjectileWrapper::setY(float y)
{
	if (pProj && isActive())
	{
		pProj->y = y;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

float ProjectileWrapper::getAngle(void)
{
	if (pProj && isActive())
	{
		return pProj->angle;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

void ProjectileWrapper::setAngle(float angle)
{
	if (pProj && isActive())
	{
		pProj->angle = angle;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

int ProjectileWrapper::getDamage(void)
{
	if (pProj && isActive())
	{
		return pProj->damage;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

void ProjectileWrapper::setDamage(int damage)
{
	if (pProj && isActive())
	{
		pProj->damage = damage;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

int ProjectileWrapper::getShieldPenetration(void)
{
	if (pProj && isActive())
	{
		return pProj->shieldPenetration;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

void ProjectileWrapper::setShieldPenetration(int penetration)
{
	if (pProj && isActive())
	{
		pProj->shieldPenetration = penetration;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

int ProjectileWrapper::getFireChance(void)
{
	if (pProj && isActive())
	{
		return pProj->fireChance;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

void ProjectileWrapper::setFireChance(int chance) {
	if (pProj && isActive())
	{
		pProj->fireChance = chance;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

int ProjectileWrapper::getBreachChance(void)
{
	if (pProj && isActive())
	{
		return pProj->breachChance;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

void ProjectileWrapper::setBreachChance(int chance)
{
	if (pProj && isActive())
	{
		pProj->breachChance = chance;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

int ProjectileWrapper::getStunChance(void)
{
	if (pProj && isActive())
	{
		return pProj->stunChance;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

void ProjectileWrapper::setStunChance(int chance)
{
	if (pProj && isActive())
	{
		pProj->stunChance = chance;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

int ProjectileWrapper::getIonDamage(void)
{
	if (pProj && isActive())
	{
		return pProj->ionDamage;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

void ProjectileWrapper::setIonDamage(int damage)
{
	if (pProj && isActive())
	{
		pProj->ionDamage = damage;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

int ProjectileWrapper::getCrewDamage(void)
{
	if (pProj && isActive())
	{
		return pProj->crewDamage;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

void ProjectileWrapper::setCrewDamage(int damage)
{
	if (pProj && isActive())
	{
		pProj->crewDamage = damage;
	}
	else
	{
		throw std::runtime_error("Projectile does not exist!");
	}
}

bool ProjectileWrapper::isActive(void)
{
	if (activeProjectiles != nullptr)
	{
		for (int i = 0; i < (*activeProjectiles)->size(); i++)
		{
			if ((**activeProjectiles)[i] == pProj)
				return true;
		}
	}
	return false;
}

// tempted to maintain my own list, but that's probably more likely to result in a UaF, so use FTL's
std::vector<chaiscript::Boxed_Value> GetActiveProjectiles()
{
	std::vector<chaiscript::Boxed_Value> output = std::vector<chaiscript::Boxed_Value>();
	if (activeProjectiles != nullptr && (*activeProjectiles) != nullptr)
	{
		for (int i = 0; i < (*activeProjectiles)->size(); i++)
		{
			output.push_back(chaiscript::var(ProjectileWrapper((**activeProjectiles)[i])));
		}
	}
	return output;
}