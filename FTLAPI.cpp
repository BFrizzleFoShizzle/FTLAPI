#include "FTLAPI.h"
#include "FTLDraw.h"
#include "TextHelper.h"

ShipWrapper *playerWrapper = new ShipWrapper();

bool inHangar(void) {
	bool *inHangar = (bool*)0x0028EECC;
	if (inHangar != NULL) {
		return *inHangar;
	}
	//TODO: maybe throw exception?
	return false;
}

void setupChai(chaiscript::ChaiScript *chai) {
	chai->add_global(chaiscript::var(playerWrapper), "playerShip");
	chai->add(chaiscript::fun(drawString2), "drawString");
	chai->add(chaiscript::fun(&ShipWrapper::getHealth), "getHealth");
	chai->add(chaiscript::fun(&ShipWrapper::getCloakImageName), "getCloakImageName");
	chai->add(chaiscript::fun(&ShipWrapper::getShipImageName), "getShipImageName");
	chai->add(chaiscript::fun(&ShipWrapper::getID), "getID");
	chai->add(chaiscript::fun(&ShipWrapper::getName), "getName");
	chai->add(chaiscript::fun(&ShipWrapper::getDisplayName), "getDisplayName");
	chai->add(chaiscript::fun(&ShipWrapper::setHealth), "setHealth");
	chai->add(chaiscript::fun(&ShipWrapper::getDroneManager), "getDroneManager");
	chai->add(chaiscript::fun(&ShipWrapper::getEngineManager), "getEngineManager");
	chai->add(chaiscript::fun(&ShipWrapper::getMedicalManager), "getMedicalManager");
	chai->add(chaiscript::fun(&ShipWrapper::getOxygenManager), "getOxygenManager");
	chai->add(chaiscript::fun(&ShipWrapper::getShieldManager), "getShieldManager");
	chai->add(chaiscript::fun(&ShipWrapper::getWeaponManager), "getWeaponManager");
	chai->add(chaiscript::fun(&UtilityManagerWrapper::getOperated), "getOperated");
	chai->add(chaiscript::fun(&UtilityManagerWrapper::getPowerLevel), "getPowerLevel");
	chai->add(chaiscript::fun(&UtilityManagerWrapper::setOperated), "setOperated");
	chai->add(chaiscript::fun(&UtilityManagerWrapper::setPowerLevel), "setPowerLevel");
	chai->add(chaiscript::fun(addDrawHook), "addDrawHook");
	chai->add(chaiscript::fun(inHangar), "inHangar");
}