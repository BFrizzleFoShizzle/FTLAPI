#include "FTLShip.h"
#include <Windows.h>
#include <stdexcept>

ship* getPlayerShip(void) {
	return playerShip;
};

ship* getEnemyShip(void) {
	//2nd element of ships is current enemy ship
	return ships+1;
};

ShipWrapper::ShipWrapper(ship* representedShip = NULL) {
	this->setShipPointer(representedShip);
}

ShipWrapper::ShipWrapper(void) {
	pShip = NULL;
}

int ShipWrapper::getHealth(void) {
	if(pShip != NULL)
		return pShip->health;
	throw std::runtime_error("Ship not created");
}

void ShipWrapper::setHealth(int newHealth) {
	if (pShip != NULL)
	{
		pShip->health = newHealth;
		return;
	}
	throw std::runtime_error("Ship not created");
}

std::string ShipWrapper::getShipImageName(void) {
	if (pShip != NULL)
		return pShip->shipImage;
	throw std::runtime_error("Ship not created");
}

std::string ShipWrapper::getCloakImageName(void) {
	if (pShip != NULL)
		return pShip->cloakImage;
	throw std::runtime_error("Ship not created");
}

std::string ShipWrapper::getID(void) {
	if (pShip != NULL)
		return pShip->id;
	throw std::runtime_error("Ship not created");
}

std::string ShipWrapper::getName(void) {
	if (pShip != NULL)
		return pShip->name;
	throw std::runtime_error("Ship not created");
}

std::string ShipWrapper::getDisplayName(void) {
	if (pShip != NULL)
		return pShip->displayName;
	throw std::runtime_error("Ship not created");
}

void ShipWrapper::setShipPointer(ship* representedShip) {
	pShip = representedShip;

	// invalidate the old objects
	if (medicalMan)
		medicalMan->setUtilityManagerPointer(NULL);
	if (droneMan)
		droneMan->setUtilityManagerPointer(NULL);
	if (engineMan)
		engineMan->setUtilityManagerPointer(NULL);
	if (weaponMan)
		weaponMan->setUtilityManagerPointer(NULL);
	if (oxygenMan)
		oxygenMan->setUtilityManagerPointer(NULL);
	if (shieldMan)
		shieldMan->setUtilityManagerPointer(NULL);

	if (pShip == NULL)
		return;

	// we're safe here, make the new power manager wrappers
	this->medicalMan = std::shared_ptr<UtilityManagerWrapper>(new UtilityManagerWrapper(pShip->medicalManager));
	this->droneMan = std::shared_ptr<UtilityManagerWrapper>(new UtilityManagerWrapper(pShip->droneManager));
	this->engineMan = std::shared_ptr<UtilityManagerWrapper>(new UtilityManagerWrapper(pShip->engineManager));
	this->weaponMan = std::shared_ptr<UtilityManagerWrapper>(new UtilityManagerWrapper(pShip->weaponManager));
	this->oxygenMan = std::shared_ptr<UtilityManagerWrapper>(new UtilityManagerWrapper(pShip->oxygenManager));
	this->shieldMan = std::shared_ptr<UtilityManagerWrapper>(new UtilityManagerWrapper(pShip->shieldManager));
}


std::shared_ptr<UtilityManagerWrapper> ShipWrapper::getOxygenManager(void) {
	if(oxygenMan == NULL)
		throw std::runtime_error("Manager does not exist!");
	return oxygenMan;
}

std::shared_ptr<UtilityManagerWrapper> ShipWrapper::getWeaponManager(void) {
	if (weaponMan == NULL)
		throw std::runtime_error("Manager does not exist!");
	return weaponMan;
}

std::shared_ptr<UtilityManagerWrapper> ShipWrapper::getDroneManager(void) {
	if (droneMan == NULL)
		throw std::runtime_error("Manager does not exist!");
	return droneMan;
}

std::shared_ptr<UtilityManagerWrapper> ShipWrapper::getEngineManager(void) {
	if (engineMan == NULL)
		throw std::runtime_error("Manager does not exist!");
	return engineMan;
}

std::shared_ptr<UtilityManagerWrapper> ShipWrapper::getMedicalManager(void) {
	if (medicalMan == NULL)
		throw std::runtime_error("Manager does not exist!");
	return medicalMan;
}

std::shared_ptr<UtilityManagerWrapper> ShipWrapper::getShieldManager(void) {
	if (shieldMan == NULL)
		throw std::runtime_error("Manager does not exist!");
	return shieldMan;
}

ship* ShipWrapper::getShipPointer(void) {
	return pShip;
}

UtilityManagerWrapper::UtilityManagerWrapper(utilityManager* representedUtil) {
	pUtil = representedUtil;
}

bool UtilityManagerWrapper::getOperated(void) {
	if(pUtil == NULL)
		throw std::runtime_error("UtilityManager no longer exists!");
	return pUtil->operated;
}

void UtilityManagerWrapper::setOperated(bool op) {
	if (pUtil == NULL)
		throw std::runtime_error("UtilityManager no longer exists!");
	pUtil->operated = op;
}

int UtilityManagerWrapper::getPowerLevel(void) {
	if (pUtil == NULL)
		throw std::runtime_error("UtilityManager no longer exists!");
	return pUtil->powerLevel;
}

void UtilityManagerWrapper::setPowerLevel(int power) {
	if (pUtil == NULL)
		throw std::runtime_error("UtilityManager no longer exists!");
	pUtil->powerLevel = power;
}

void UtilityManagerWrapper::setUtilityManagerPointer(utilityManager* representedUtil) {
	pUtil = representedUtil;
}

utilityManager* UtilityManagerWrapper::getUtilityManagerPointer(void) {
	return pUtil;
}