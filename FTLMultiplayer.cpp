#include "FTLMultiplayer.h"
#include "TextHelper.h"
#include "FTLShipSelector.h"
#include "FTLDraw.h"
#include "FTLAPI.h"
#include <cstdlib>
#include <windows.h>
#include <cstdio>
#include <ctime>
#include <gl\GL.h>
#include <chaiscript\chaiscript.hpp>
#include <chaiscript\chaiscript_stdlib.hpp>


#define DEBUG true
//black magic for getting a handle on our own DLL
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());

HANDLE FTLProcess;


void drawShit(void) {
	char output[10];
	if(playerShip != NULL) {
		//error checking is for bitches
		//int input = chai.eval<int>("playerShip.health");
		int input = 0;
		try {
			if (playerWrapper) {
				
			}
		}
		catch (const std::exception& e) {
			MessageBox(NULL, e.what(), "test", MB_OK + MB_ICONINFORMATION);
			
		}
	}
};

DWORD WINAPI FTLM_Main (LPVOID lpParam)
{
	char output[50];
	FTLProcess = GetCurrentProcess();
	//this will also hook glfinish
	FTLSSMain();
	//add our draw hook
	addDrawHook(drawShit);
	//set up chai
	setupChai(&chai);
	//execute test script
	chai.eval_file("test.chai");
	//set up pointers to game stuff
	while(playerShip == NULL){
		ReadProcessMemory(FTLProcess,(VOID*)(0x400000+0x39BA90),&playerShip,4,NULL);
		Sleep(100);
	}
	playerWrapper->setShipPointer(playerShip);
	return 0;
};

BOOL WINAPI DllMain (HINSTANCE hModule, DWORD dwAttached, LPVOID lpvReserved)
{
    if (dwAttached == DLL_PROCESS_ATTACH) {
        CreateThread(NULL,0,&FTLM_Main,NULL,0,NULL);
    }
    return 1;
};