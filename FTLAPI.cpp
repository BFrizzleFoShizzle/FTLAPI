#include "FTLAPI.h"
#include "FTLDraw.h"
#include "TextHelper.h"
#include "FTLShipSelector.h"
#include <SDL.h>
//#include <SDL_events.h>

ShipWrapper *playerShipWrapper = new ShipWrapper();

std::vector<std::function<void(void)>> mouseDownHooks;
std::vector<std::function<void(void)>> mouseUpHooks;
std::vector<std::function<void(int)>> keyUpHooks;
std::vector<std::function<void(int)>> keyDownHooks;

bool inHangar(void) {
	bool *inHangar = (bool*)(mainStackBase - 0x1144);
	if (inHangar != NULL) {
		return *inHangar;
	}
	//TODO: maybe throw exception?
	return false;
}

struct Point {
	int x;
	int y;
};

void messageBoxBlock(std::string text, std::string title) {
	MessageBox(NULL, text.c_str(), title.c_str(), MB_OK + MB_ICONINFORMATION);
}

void messageBox(std::string text, std::string title) {
	std::thread msgBox(messageBoxBlock, text, title);
	msgBox.detach();
}

void addMouseUpHook(std::function<void(void)> function) {
	mouseUpHooks.push_back(function);
}

void addMouseDownHook(std::function<void(void)> function) {
	mouseDownHooks.push_back(function);
}

void addKeyUpHook(std::function<void(int)> function) {
	keyUpHooks.push_back(function);
}

void addKeyDownHook(std::function<void(int)> function) {
	keyDownHooks.push_back(function);
}


std::shared_ptr<Point> getCursorPos(void) {
	std::shared_ptr<Point> p = std::shared_ptr<Point>(new Point());
	SDL_GetMouseState(&p->x, &p->y);
	return p;
}

int eventFilter (SDL_Event* event){
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		for (int i = 0; i < mouseDownHooks.size(); i++) {
			try {
				mouseDownHooks[i]();
			}
			catch (std::exception e) {
				messageBox(e.what(), "Mouse hook fail!");
			}
		}
	}
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
		for (int i = 0; i < mouseDownHooks.size(); i++) {
			try {
				mouseUpHooks[i]();
			}
			catch (std::exception e) {
				messageBox(e.what(), "Mouse hook fail!");
			}
		}
	}
	if (event->type == SDL_KEYDOWN) {
		for (int i = 0; i < keyDownHooks.size(); i++) {
			try {
				keyDownHooks[i](event->key.keysym.sym);
			}
			catch (std::exception e) {
				messageBox(e.what(), "KeyDown hook fail!");
			}
		}
	}
	if (event->type == SDL_KEYUP) {
		for (int i = 0; i < keyUpHooks.size(); i++) {
			try {
				keyUpHooks[i](event->key.keysym.sym);
			}
			catch (std::exception e) {
				messageBox(e.what(), "KeyUp hook fail!");
			}
		}
	}
	return 1;
}



void setupChai(chaiscript::ChaiScript *chai) {
	// FTL doesn't use an event filter, so this is OK
	SDL_SetEventFilter((SDL_EventFilter)eventFilter);
	// Key repeat (useful for text input, etc)
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	chai->add_global(chaiscript::var(playerShipWrapper), "playerShip");
	chai->add(chaiscript::fun(drawString2), "drawString");
	chai->add(chaiscript::fun(drawRect), "drawRect");
	chai->add(chaiscript::fun(messageBoxBlock), "messageBox_block");
	chai->add(chaiscript::fun(messageBoxBlock), "msg_block");
	chai->add(chaiscript::fun(messageBox), "messageBox");
	chai->add(chaiscript::fun(messageBox), "msg");
	chai->add(chaiscript::fun(setColor), "setColor");
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
	chai->add(chaiscript::fun(addMouseDownHook), "addMouseDownHook");
	chai->add(chaiscript::fun(addMouseUpHook), "addMouseUpHook");
	chai->add(chaiscript::fun(addKeyDownHook), "addKeyDownHook");
	chai->add(chaiscript::fun(addKeyUpHook), "addKeyUpHook");
	chai->add(chaiscript::fun(getCursorPos), "getCursorPos");
	chai->add(chaiscript::fun(&Point::x), "x");
	chai->add(chaiscript::fun(&Point::y), "y");
	chai->add(chaiscript::fun(&SDL_GetTicks), "getTicks");
	chai->add(chaiscript::fun(inHangar), "inHangar");
}