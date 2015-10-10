#include "FTLAPI.h"
#include "FTLDraw.h"
#include "TextHelper.h"
#include "FTLShipSelector.h"

ShipWrapper *playerWrapper = new ShipWrapper();

std::vector<std::function<void(int)>> keyUpHooks;
std::vector<std::function<void(int)>> keyDownHooks;

bool inHangar(void) {
	bool *inHangar = (bool*)0x0028EECC;
	if (inHangar != NULL) {
		return *inHangar;
	}
	//TODO: maybe throw exception?
	return false;
}

HHOOK hKeyboardHook;

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	char window_title[5];
	HWND foreground = GetForegroundWindow();
	GetWindowText(foreground, window_title, 5);
	// ignore if not our window (should make this better...)
	KBDLLHOOKSTRUCT *params = (KBDLLHOOKSTRUCT*)lParam;
	if (strcmp(window_title, "FTL") == 0) {
		if (wParam == WM_KEYDOWN)
		{
			for (int i = 0; i < keyDownHooks.size(); i++) {
				try {
					keyDownHooks[i](params->vkCode);
				}
				catch (std::exception e) {
					messageBox(e.what(), "KeyDown hook fail!");
				}
			}
		}
		if (wParam == WM_KEYUP)
		{
			for (int i = 0; i < keyUpHooks.size(); i++) {
				try {
					keyUpHooks[i](params->vkCode);
				}
				catch (std::exception e) {
					messageBox(e.what(), "KeyUp hook fail!");
				}
			}
		}
	}
	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

void addKeyUpHook(std::function<void(int)> function) {
	keyUpHooks.push_back(function);
}

void addKeyDownHook(std::function<void(int)> function) {
	keyDownHooks.push_back(function);
}

void setupChai(chaiscript::ChaiScript *chai) {
	chai->add_global(chaiscript::var(playerWrapper), "playerShip");
	chai->add(chaiscript::fun(drawString2), "drawString");
	chai->add(chaiscript::fun(drawRect), "drawRect");
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
	chai->add(chaiscript::fun(addKeyDownHook), "addKeyDownHook");
	chai->add(chaiscript::fun(addKeyUpHook), "addKeyUpHook");
	chai->add(chaiscript::fun(inHangar), "inHangar");
}