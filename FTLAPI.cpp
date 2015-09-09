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
	chai->add(chaiscript::fun(&ShipWrapper::setHealth), "setHealth");
	chai->add(chaiscript::fun(addDrawHook), "addDrawHook");
	chai->add(chaiscript::fun(inHangar), "inHangar");
}