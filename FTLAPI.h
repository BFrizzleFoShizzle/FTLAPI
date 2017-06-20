#pragma once
#include <Windows.h>
#include <chaiscript\chaiscript.hpp>
#include "FTLShip.h"

//just a simple interoperability header so that I don't end up with two sets of variables
extern HANDLE FTLProcess;

extern ShipWrapper *playerShipWrapper;

extern HINSTANCE hInstance;

// char so that offsets are byte-size
extern char* mainStackBase;

void setupChai(chaiscript::ChaiScript *chai);

