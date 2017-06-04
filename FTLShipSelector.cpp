#include "FTLShipSelector.h"
#include "FTLAPI.h"
#include <cstdlib>
#include <gl\GL.h>
#include "TextHelper.h"
#include "FTLDraw.h"
#include "DirtyHooker.h"
#include "ShipDescriptor.h"

//black magic for getting a handle on our own DLL
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

DWORD mouseClickPointer = 0x00400000+0x22BF3C;

char ourDirectory[MAX_PATH] = {0};

ShipDescriptor* selectedShip = NULL;
ShipDescriptor* firstShip = NULL;
int* selectedShipBase = NULL;

HWND ftlWindow = NULL;

char output[500];

//random function for DLL export so I can
//add the DLL to FTL's import table
__declspec(dllexport) void ayyLmao() {

}

//select next ship
void shipRotForward() {
	char* lastShipID = selectedShip->shipID2;
	if(selectedShip->greaterThan!=NULL) {
		//go to the least child of the greater child
		selectedShip = selectedShip->greaterThan;
		while(selectedShip->lessThan!=NULL){
			selectedShip = selectedShip->lessThan;
		}
	} else {
		//gotta move up
		while(true) {
			//loop till we're at the top, or at a greater node
			if(strcmp(selectedShip->shipID2,lastShipID)>0) {
				//found a parent ship higher than us
				break;
			} else{
				if(selectedShip != firstShip) {
					selectedShip = selectedShip->parent;
				} else {
					//if we're at the top and NOT a greater node,
					//we must go back to the beginning of the list
					while(selectedShip->greaterThan!=NULL) {
						selectedShip = selectedShip->lessThan;
					}
					break;
				}
			}
		}
	}
	selectedShipBase = &(selectedShip->base);
}
/*
DWORD WINAPI mouseHookLoop (LPVOID lpParam) {
	while(true) {
		MSG msg;
		if (PeekMessage(&msg,0,0,0,PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}
	return 0;
}
*/
void shipRotBackward() {
	char* lastShipID = selectedShip->shipID2;
	if(selectedShip->lessThan!=NULL) {
		//go to the greatest child of the lesser child
		selectedShip = selectedShip->lessThan;
		while(selectedShip->greaterThan!=NULL){
			selectedShip = selectedShip->greaterThan;
		}
	} else {
		//gotta move up
		while(true) {
			//loop till we're at the top, or at a lesser node
			if(strcmp(selectedShip->shipID2,lastShipID)<0) {
				//found a parent ship higher than us
				break;
			} else{
				if(selectedShip != firstShip) {
					selectedShip = selectedShip->parent;
				} else {
					//if we're at the top and NOT a lesser node, we must
					//go back to the beginning of the list
					while(selectedShip->greaterThan!=NULL) {
						selectedShip = selectedShip->greaterThan;
					}
					break;
				}
			}
		}
	}
	selectedShipBase = &(selectedShip->base);
}

void rotForwardPlayerShip() {
	//prefix for player ships
	char prefix[] = "PLAYER_";
	shipRotForward();
	//if it's not a player ship, start again from the beginning!
	//loop until player again
	while(strncmp(selectedShip->shipID2,prefix,7)!=0) {
		shipRotForward();
	}
}

void rotBackwardPlayerShip() {
	//prefix for player ships
	char prefix[] = "PLAYER_";
	shipRotBackward();
	//if it's not a player ship, start again from the beginning!
	//loop until player again
	while(strncmp(selectedShip->shipID2,prefix,7)!=0) {
		shipRotBackward();
	}
}

//Don't inline or it'll mess up the hook's stack frame!
__declspec(noinline) void dealWithClick(void) {
	int x, y;
	__asm
	{
		//copy values to x and y
		push eax;
		mov eax, [ebp+0x14];
		mov x, eax;
		mov eax, [ebp+0x18];
		mov y, eax;
		pop eax
	}
	if(ftlWindow==NULL){
		/*ftlWindow = GetActiveWindow(); 
		HINSTANCE hInstance = (HINSTANCE)GetWindowLong(ftlWindow, GWL_HINSTANCE);
		DWORD threadID = GetWindowThreadProcessId(ftlWindow, NULL);
		sprintf(output, "threadID %lu hInstance %lu", threadID, hInstance);
		MessageBox(NULL, output, "test", MB_OK + MB_ICONINFORMATION);
		//DWORD threadID = GetCurrentThreadId();
		SetWindowsHookEx(WH_MOUSE, MouseHookProc, hInstance, NULL);*/
		//sprintf(output,"FTLWindow %x",ftlWindow);
		//MessageBox(NULL, output, "test", MB_OK + MB_ICONINFORMATION);
		//GetWindowText(ftlWindow,output,50);
		//MessageBox(NULL, output, "test", MB_OK + MB_ICONINFORMATION);
	}
	//check if in hangar (stack grows down, negative offset)
	if(*((bool*)(mainStackBase-0x1144))) {
		//button 1
		if(x>25 && x<95 && y>175 && y<210) {
			rotBackwardPlayerShip();
		}
		//button 2
		if(x>95 && x<165 && y>175 && y<210) {
			rotForwardPlayerShip();
		}
	}
	char out[50];
	sprintf(out,"X: %i, Y: %i", x, y);
	//MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);

}

void hookClick(void){
	__asm
	{
		//undo VC++ crap
		pop edi;
		pop ebx;
		//apply overwritten code
		movzx edx,word ptr [ebx+04];
		mov [esp], edx;
		//prep for func call:
		push eax;
		push ecx;
		push edx;
	}
	dealWithClick();
	__asm
	{
		//recover from function call:
		pop edx;
		pop ecx;
		pop eax;
		//do a push ret back to the code (8 bytes ahead of where we were
		push mouseClickPointer;
		push eax;
		mov eax, [esp+4];
		add eax, 7;
		mov [esp+4], eax;
		pop eax;
		ret;
	}
}

void drawStuff(void) {
	/*__asm{
		int 3;
	}*/
	if(DEBUG) {
		ShipDescriptor* currShip = firstShip;
		float y = 60.0f;
		sprintf_s(output,"0 %x", selectedShipBase);
		drawString(200.0f,y,output);
		y+=20.0f;
		if(selectedShip!=NULL)
			sprintf_s(output,"1 %x %s", selectedShip, selectedShip->shipID2);
		drawString(200.0f,y,output);
		y+=20.0f;
		if(firstShip!=NULL)
			sprintf_s(output,"2 %x %s", firstShip, firstShip->shipClass);
		drawString(200.0f,y,output);
	}
	//if in hangar, draw custom UI stuff
	if(*((bool*)mainStackBase - 0x1144)) {
		//(5, 110) (185, 110) (185, 340) (5, 340)
		//glColor3f(0.5,0.5,0.5);
		//drawRect(5,110,180,231);
		glColor3f(0.3,0.3,0.3);
		//drawRect(25,175,140,35);
		drawRect(19,175,153,35);
		glColor4f(1,1,1,1);
		drawString(30,190,"Previous");
		drawString(115,190,"Next");
		//draw a cursor if we're covering it...
		if(ftlWindow!=NULL) {
			//3x3 square at cursor so the user knows where it is...
			glColor4f(1,1,1,1);
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(ftlWindow,&p);
			if(DEBUG) {
				sprintf(output,"Mousepos %i %i",p.x, p.y);
				drawString(200,190,output);
			}
			if(p.x>5&&p.x<185&&p.y>110&&p.y<340){
				//draw cursor
				drawTriangle(p.x,p.y,p.x+10,p.y+10,p.x,p.y+14);
			}
		}
	}
};

void FTLSSMain (void)
{
	FTLProcess = GetCurrentProcess();
	// Hook openGL finish
	hookGLFinish();
	addDrawHook(drawStuff);
	//hook mouse click
	RETHook6Byte(mouseClickPointer,hookClick,FTLProcess);
	//font loading stuff
	//get our DLL's path, in a useful form
	WCHAR wcDllPath[MAX_PATH] = {0};
	GetModuleFileNameW((HINSTANCE)&__ImageBase, wcDllPath, _countof(wcDllPath));
	wcstombs(ourDirectory,wcDllPath,MAX_PATH);
	//strip DLL name
	*(strrchr(ourDirectory, '\\')+1) = '\x00';
	//work out file path to bitmap font
	char bitmapFile[MAX_PATH];
	FILE* fp;
	sprintf(bitmapFile, "%sfont.bmp", ourDirectory);
	fp = fopen(bitmapFile, "r");
	if(!fp) {
		MessageBox(NULL, "Missing fonts file! D: should be...", "Fail!", MB_OK + MB_ICONINFORMATION);
		MessageBox(NULL, bitmapFile, "Fail!", MB_OK + MB_ICONINFORMATION);
	}
	//load the bitmap font from our dir
	readBitmapFont(fp);

	// set up pointers to game stuff
	// wait for ships to be initialized
	while(true){
		ShipDescriptor* shipsDescExe;
		ReadProcessMemory(FTLProcess,(VOID*)(playerShipsExe),&shipsDescExe,4,NULL);
		// check if ships have been loaded
		char out[50];
		if(DEBUG){
			sprintf(out,"DescExe %x",shipsDescExe);
			MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
		}
		Sleep(1000);
		if(shipsDescExe && shipsDescExe->base == 0x007923C8) {
			// descriptors are loaded, we can get the value and safely hook the functions now!
			firstShip = shipsDescExe;
			selectedShip = shipsDescExe;
			selectedShipBase = &(shipsDescExe->base);
			// overwrite instruction that gets player ship descriptor for ship creation
			// this means it gets our value instead of the game's value
			MovAddrReg6Byte(0x00400000+0x001D6B4A,"eax",&selectedShipBase);
			// NOP 1 byte (replaced instructions are 7 bytes, hook is 6)
			NOP(0x00400000+0x001D6B4A+6,1);
			break;
		}
	}
};