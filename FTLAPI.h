#pragma once
#include <Windows.h>
#include <chaiscript\chaiscript.hpp>
#include "FTLShip.h"

//just a simple interoperability header so that I don't end up with two sets of variables
extern HANDLE FTLProcess;

extern ShipWrapper *playerWrapper;

extern HINSTANCE hInstance;

void setupChai(chaiscript::ChaiScript *chai);

