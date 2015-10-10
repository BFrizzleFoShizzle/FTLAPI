#include "FTLAPI.h"
#include "FTLDraw.h"
#include "TextHelper.h"
#include "FTLShipSelector.h"

ShipWrapper *playerWrapper = new ShipWrapper();

std::vector<std::function<void(void)>> mouseDownHooks;
std::vector<std::function<void(void)>> mouseUpHooks;
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

HHOOK hMouseHook;
HHOOK hKeyboardHook;

void messageBoxBlock(std::string text, std::string title) {
	MessageBox(NULL, text.c_str(), title.c_str(), MB_OK + MB_ICONINFORMATION);
}

void messageBox(std::string text, std::string title) {
	std::thread msgBox(messageBoxBlock, text, title);
	msgBox.detach();
}

LRESULT CALLBACK mouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	char window_title[5];
	HWND foreground = GetForegroundWindow();
	GetWindowText(foreground, window_title, 5);
	// ignore if not our window (should make this better...)
	if (strcmp(window_title, "FTL")==0) {
		if (wParam == WM_LBUTTONDOWN)
		{
			for (int i = 0; i < mouseDownHooks.size(); i++) {
				try {
					mouseDownHooks[i]();
				}
				catch (std::exception e) {
					messageBox(e.what(), "Mouse hook fail!");
				}
			}
		}
		if (wParam == WM_LBUTTONUP)
		{
			for (int i = 0; i < mouseUpHooks.size(); i++) {
				try {
					mouseUpHooks[i]();
				}
				catch (std::exception e) {
					messageBox(e.what(), "Mouse hook fail!");
				}
			}
		}
	}
	return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

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

DWORD WINAPI hookMouse(LPVOID lpParam) {
	ftlWindow = FindWindow(NULL, "FTL");
	//HINSTANCE hInstance = GetModuleHandle(NULL);
	//set mouse hook
	DWORD threadID = GetWindowThreadProcessId(ftlWindow, NULL);
	//DWORD threadID = GetCurrentThreadId();
	hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseHookProc, hInstance, NULL);
	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, hInstance, NULL);
	//MessageBox(NULL, output, "test", MB_OK + MB_ICONINFORMATION);
	MSG message;
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return 0;
}


std::shared_ptr<POINT> getCursorPos(void) {
	if (ftlWindow == NULL)
		throw std::runtime_error("Mouse not hooked!");
	LPPOINT p = new POINT();
	if (GetCursorPos(p)) {
		if (ScreenToClient(ftlWindow, p)) {
			std::shared_ptr<POINT> pt = std::shared_ptr<POINT>(p);
			return pt;
		}
		throw std::runtime_error("ScreenToClient fail!");
	}
	throw std::runtime_error("GetCursorPos fail!");
}

void setupChai(chaiscript::ChaiScript *chai) {
	CreateThread(NULL, 0, &hookMouse, NULL, 0, NULL);
	chai->add_global(chaiscript::var(playerWrapper), "playerShip");
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
	chai->add(chaiscript::fun(&POINT::x), "x");
	chai->add(chaiscript::fun(&POINT::y), "y");
	chai->add(chaiscript::fun(inHangar), "inHangar");
}