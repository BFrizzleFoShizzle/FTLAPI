#include "FTLMultiplayer.h"
#include "TextHelper.h"
#include "FTLShipSelector.h"
#include "FTLDraw.h"
#include "FTLAPI.h"
#include "DirtyHooker.h"
#include <cstdlib>
#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>
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

HINSTANCE hInstance;

char* mainStackBase;


void UpdatePlayerShip(void) {
	playerShipWrapper->setShipPointer(playerShip);
}

// returns a HookAddr containing the hook code defined in the function
__declspec(naked) void PlayerShipCreateHook(void) {
	__asm {
		//hook asm
		mov[playerShip], eax;
		ret;
	}
}

DWORD GetMainThreadId()
{
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (h != INVALID_HANDLE_VALUE) {
		THREADENTRY32 te;
		te.dwSize = sizeof(te);
		if (Thread32First(h, &te)) {
			do {
				if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) +
					sizeof(te.th32OwnerProcessID) &&
					te.th32OwnerProcessID == GetCurrentProcessId()) {
					return te.th32ThreadID;
				}
				te.dwSize = sizeof(te);
			} while (Thread32Next(h, &te));
		}
		CloseHandle(h);
	}
	
	return 0;
}

typedef LONG KPRIORITY;

typedef struct _CLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID;

typedef struct THREAD_BASIC_INFORMATION
{
	NTSTATUS                ExitStatus;
	NT_TIB*                 TebBaseAddress;
	CLIENT_ID               ClientId;
	KAFFINITY               AffinityMask;
	KPRIORITY               Priority;
	KPRIORITY               BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

void* GetMainThreadStackBase()
{
	int threadID = GetMainThreadId();
	HANDLE mainThread = OpenThread(THREAD_ALL_ACCESS, false, threadID);
	
	THREAD_BASIC_INFORMATION basicInfo = { 0 };
	
	bool loadedManually = false;
	HMODULE module = GetModuleHandle("ntdll.dll");

	if (!module)
	{
		module = LoadLibrary("ntdll.dll");
		loadedManually = true;
	}
	//SuspendThread(threadID);

	NTSTATUS(__stdcall *NtQueryInformationThread)(HANDLE ThreadHandle, THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength);
	NtQueryInformationThread = reinterpret_cast<decltype(NtQueryInformationThread)>(GetProcAddress(module, "NtQueryInformationThread"));

	// Get TEB address
	NtQueryInformationThread(mainThread, (THREADINFOCLASS)0, &basicInfo, sizeof(THREAD_BASIC_INFORMATION), NULL);
	if (loadedManually)
	{
		FreeLibrary(module);
	}
	// Read stack base from TEB/TIB
	return basicInfo.TebBaseAddress->StackBase;
}

DWORD WINAPI FTLM_Main (LPVOID lpParam)
{
	// find the main thread's stack
	mainStackBase = (char*)GetMainThreadStackBase();

	FTLProcess = GetCurrentProcess();
	//this will also hook glfinish
	FTLSSMain();
	//add ship creation hook
	// could hook directly after player ship constructor at 0x400000 + 0x1DCB85, but ship is NOT fully created at that point...
	DWORD playerShipCreationAddr = 0x400000 + 0x6314C; // 6 bytes
	//function hook first, this means it gets called after the naked hook
	RET6AutoHookFunction(UpdatePlayerShip, playerShipCreationAddr, 6, FTLProcess);
	RET6CALL5AutoHookNaked(PlayerShipCreateHook, playerShipCreationAddr, 6, FTLProcess);
	//set up chai
	setupChai(&chai);
	//execute test script
	try {
		chai.eval_file("test.chai");
	}
	catch (std::exception e) {
		MessageBox(NULL, e.what(), "Script error!", MB_OK + MB_ICONINFORMATION);
	}
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
	hInstance = hModule;
    if (dwAttached == DLL_PROCESS_ATTACH) {
        CreateThread(NULL,0,&FTLM_Main,NULL,0,NULL);
    }
    return 1;
};