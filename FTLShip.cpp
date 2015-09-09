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
	pShip = representedShip;
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
		pShip->health = newHealth;
	throw std::runtime_error("Ship not created");
}

void ShipWrapper::setShipPointer(ship* representedShip) {
	pShip = representedShip;
}

ship* ShipWrapper::getShipPointer(void) {
	return pShip;
}