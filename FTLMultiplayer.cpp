#include "FTLMultiplayer.h"
#include "TextHelper.h"
#include "FTLShipSelector.h"
#include "FTLDraw.h"
#include "FTLAPI.h"
#include "DirtyHooker.h"
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


void updateShip(void) {
	playerWrapper->setShipPointer(playerShip);
}

// returns a HookAddr containing the hook code defined in the function
HookAddr getShipCreateHook(void) {
	HookAddr output;
	void* p1;
	void* p2;
	__asm {
		//jump to hook addr calculation code
		jmp hookEnd;
	hookStart:
		//hook asm
		mov [playerShip], eax;
	hookEnd:
		mov [p1], offset hookStart;
		mov [p2], offset hookEnd;

	}
	output.startAddress = (DWORD)p1;
	output.endAddress = (DWORD)p2;
	return output;
}

DWORD WINAPI FTLM_Main (LPVOID lpParam)
{
	FTLProcess = GetCurrentProcess();
	//this will also hook glfinish
	FTLSSMain();
	//add ship creation hook
	DWORD shipCreationAddr = 0x400000 + 0x6314C;
	//function hook first, this means it gets called after the ASM block hook
	RET6AutoHookFunction(updateShip, shipCreationAddr, 6, FTLProcess);
	RET6AutoHookASMBlock(getShipCreateHook(), shipCreationAddr, 6, FTLProcess);
	//set up chai
	setupChai(&chai);
	//execute test script
	chai.eval_file("test.chai");
	//set up pointers to game stuff
	/*while(playerShip == NULL){
		ReadProcessMemory(FTLProcess,(VOID*)(0x400000+0x39BA90),&playerShip,4,NULL);
		Sleep(100);
	}
	playerWrapper->setShipPointer(playerShip);*/
	return 0;
};

BOOL WINAPI DllMain (HINSTANCE hModule, DWORD dwAttached, LPVOID lpvReserved)
{
    if (dwAttached == DLL_PROCESS_ATTACH) {
        CreateThread(NULL,0,&FTLM_Main,NULL,0,NULL);
    }
    return 1;
};