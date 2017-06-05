#include "DirtyHooker.h"
#include <cstdio>

// changes instruction to be MOV reg, *value
void MovAddrReg6Byte(DWORD instruction, char* const reg, void* pValue, HANDLE process){
	BYTE hook[6] = {0x8B, 0x00, 0x00, 0x00, 0x00, 0x00};
	if(strcmp(reg, "ecx")==0){
		hook[1]=0x0D;
	} 
	else if(strcmp(reg, "ebx")==0){
		hook[1]=0x1D;
	}
	else if(strcmp(reg, "ebp")==0){
		hook[1]=0x2D;
	}
	else if(strcmp(reg, "edi")==0){
		hook[1]=0x3D;
	}
	else if(strcmp(reg, "eax")==0){
		hook[1]=0x05;
	}
	else if(strcmp(reg, "edx")==0){
		hook[1]=0x15;
	}
	else if(strcmp(reg, "esp")==0){
		hook[1]=0x25;
	}
	else if(strcmp(reg, "esi")==0){
		hook[1]=0x35;
	} else {
		return;
	}
	hook[2] = ((DWORD)pValue>>0)&0xFF;
	hook[3] = ((DWORD)pValue>>8)&0xFF;
	hook[4] = ((DWORD)pValue>>16)&0xFF;
	hook[5] = ((DWORD)pValue>>24)&0xFF;
	char out[50];
	if(DEBUG){
		sprintf(out,"instructions: %02X:%02X:%02X:%02X:%02X:%02X",hook[0],hook[1],hook[2],hook[3],hook[4],hook[5]);
		MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
	}
	SIZE_T bytesWritten = 0;
	int success = WriteProcessMemory(process, (VOID*)(instruction), &hook, sizeof(hook), &bytesWritten);
}

//replaces instruction with  push &function, return (jumps to first instruction in function)
//this jumps execution to the function pointed at by the given pointer
//example usage: RETHook6Byte(0x12345678, functionName);
__declspec(noinline) void RETHook6Byte(DWORD instruction, void* function, HANDLE process) {
	BYTE hook[6] = {0x68, 0x00, 0x00, 0x00, 0x00, 0xC3};
	//write function address to PUSH instruction, bytes are in reverse order
	hook[1] = ((DWORD)function>>0)&0xFF;
	hook[2] = ((DWORD)function>>8)&0xFF;
	hook[3] = ((DWORD)function>>16)&0xFF;
	hook[4] = ((DWORD)function>>24)&0xFF;
	// write hook to process memory, create popup on success or failiure if debug
	char message[50];
	SIZE_T bytesWritten = 0;
	int success = WriteProcessMemory(process, (VOID*)(instruction), &hook, sizeof(hook), &bytesWritten);
	if(DEBUG) {
		char out[50];
		sprintf(out,"Test %i",bytesWritten);
		MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
	}
}

//replaces instruction with  call &function (near, relative)
//example usage: CALLHook5Byte(0x12345678, functionName);
void CALLHook5Byte(DWORD instruction, void* function, HANDLE process) {
	DWORD offset = ((DWORD)function) - (instruction + 5);
	BYTE hook[5] = {0xE8, 0x00, 0x00, 0x00, 0x00};
	//write function address to PUSH instruction, bytes are in reverse order
	hook[1] = ((DWORD)offset >>0)&0xFF;
	hook[2] = ((DWORD)offset >>8)&0xFF;
	hook[3] = ((DWORD)offset >>16)&0xFF;
	hook[4] = ((DWORD)offset >>24)&0xFF;
	// write hook to process memory, create popup on success or failiure if debug
	char message[50];
	SIZE_T bytesWritten = 0;
	int success = WriteProcessMemory(process, (VOID*)instruction, &hook, sizeof(hook), &bytesWritten);
	char out[50];
	if(DEBUG){
		sprintf(out,"Test %i",bytesWritten);
		MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
	}
}

// replaces instruction with  call function (near, absolute)
// example usage: SafeCALLHook21Byte(0x12345678, functionName);
// this hook will protect the state of all registers
void SafeCALLHook21Byte(DWORD instruction, void* function, HANDLE process) {
	//push all (important) registers, make call, pop all (important) registers
	BYTE hook[21] = { 0x50, 0x51, 0x52, 0x53, 0x55, 0x56, 0x57, 0xB8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xD0, 0x5F, 0x5E, 0x5D, 0x5B, 0x5A, 0x59, 0x58 };
	//write function address to PUSH instruction, bytes are in reverse order
	hook[8] = ((DWORD)function >> 0) & 0xFF;
	hook[9] = ((DWORD)function >> 8) & 0xFF;
	hook[10] = ((DWORD)function >> 16) & 0xFF;
	hook[11] = ((DWORD)function >> 24) & 0xFF;
	// write hook to process memory, create popup on success or failiure if debug
	char message[50];
	SIZE_T bytesWritten = 0;
	int success = WriteProcessMemory(process, (VOID*)instruction, &hook, sizeof(hook), &bytesWritten);
	char out[50];
	if (DEBUG) {
		sprintf(out, "Test %i", bytesWritten);
		MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
	}
}

// Automagically generates the code for linking up the ASM instructions specified by hook
// at replacedInstruction, including copying and linking up the replaced instructions so
// that they still get executed after your hook code.
// The replaced instruction(s) MUST be greater than 6 bytes
// replacedInstructionsLen MUST be greater than 6, and should be the length required to replace
// whole instructions e.g. if the first instruction at the address is 4 bytes long, and the second
// is 5 bytes long, replacedInstructionsLen must be 9 (4<6, 4+5=9, !9<6, ok) in order to not 
// break these instructions
void RET6AutoHookASMBlock(HookAddr hook, DWORD replacedInstruction, int replacedInstructionsLen, HANDLE process) {
	// add jmps so game code -> hook code -> backup code -> game code
	char out[150];
	// allocate memory for copied instructions
	BYTE* instructionsCopy = (BYTE*)VirtualAlloc(NULL,replacedInstructionsLen + 6, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	// copy (to be) overwritten instructions
	SIZE_T bytesWritten = 0;
	ReadProcessMemory(process, (VOID*)(replacedInstruction), instructionsCopy, replacedInstructionsLen, &bytesWritten);
	if (bytesWritten != replacedInstructionsLen) {
		sprintf(out, "Failed hook 1: %x, only wrote: %i, should have written: %i", replacedInstruction, bytesWritten, replacedInstructionsLen);
		MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
	}
	// now we have copied the soon-to-be-overwritten bytes,
	// use a RET hook to jump back to the program
	RETHook6Byte(((DWORD)instructionsCopy)+replacedInstructionsLen, (void*)(replacedInstruction + replacedInstructionsLen), process);
	// write hook to process memory, create popup on success or failiure if debug

	int hookLen = hook.endAddress - hook.startAddress;
	BYTE* hookCopy = (BYTE*)VirtualAlloc(NULL, hookLen + 6, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	// copy our hook code, and make execution go to the backup
	memcpy(hookCopy, (void*)hook.startAddress, hookLen);
	//use RET hook to jump to copied instructions
	RETHook6Byte((DWORD)&hookCopy[hookLen], instructionsCopy, process);
	// FINALLY, add the ret hook to the program's code, going to the hook code
	// and NOP the rest of the copied bytes so it dissassembles nicely ;)
	RETHook6Byte(replacedInstruction, hookCopy, process);
	NOP(replacedInstruction+6, replacedInstructionsLen - 6, process);
}

// Automagically generates the code for linking up the naked function specified by hook
// at replacedInstruction, including copying and linking up the replaced instructions so
// that they still get executed after your hook code.
// Note: because the naked function is executed with a call instruction, the stack is offet 4 bytes.
// The naked function is responsible for maintaining the callers registers and stack
// The replaced instruction(s) MUST be greater than 5 bytes
// replacedInstructionsLen MUST be greater than 5, and should be the length required to replace
// whole instructions e.g. if the first instruction at the address is 4 bytes long, and the second
// is 5 bytes long, replacedInstructionsLen must be 9 (4<5, 4+5=9, !9<6, ok) in order to not 
// break these instructions
void RET6CALL5AutoHookNaked(void* hook, DWORD replacedInstruction, int replacedInstructionsLen, HANDLE process) {
	// add jmps so game code -> hook code -> backup code -> game code
	char out[150];
	// allocate memory for copied instructions
	BYTE* instructionsCopy = (BYTE*)VirtualAlloc(NULL, replacedInstructionsLen+5+6, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	// CALL hook to naked function
	CALLHook5Byte((DWORD)(instructionsCopy), hook, process);
	// copy (to be) overwritten instructions
	SIZE_T bytesWritten = 0;
	ReadProcessMemory(process, (VOID*)(replacedInstruction), instructionsCopy+5, replacedInstructionsLen, &bytesWritten);
	if (bytesWritten != replacedInstructionsLen) {
		sprintf(out, "Failed hook 1: %x, only wrote: %i, should have written: %i", replacedInstruction, bytesWritten, replacedInstructionsLen);
		MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
	}
	//use RET to jump back to game's code after hook
	RETHook6Byte((DWORD)(instructionsCopy + replacedInstructionsLen + 5), (void*)(replacedInstruction + replacedInstructionsLen), process);
	// RET from game's code to hook start
	RETHook6Byte(replacedInstruction, instructionsCopy, process);
	NOP(replacedInstruction + 6, replacedInstructionsLen - 6, process);
}

// Function is safely called
// Automagically generates the code for linking up the function , including copying and linking up
// the replaced instructions so that they still get executed after your hook code.
// The replaced instruction(s) MUST be greater than 6 bytes
// replacedInstructionsLen MUST be greater than 6, and should be the length required to replace
// whole instructions e.g. if the first instruction at the address is 4 bytes long, and the second
// is 5 bytes long, replacedInstructionsLen must be 9 (4<6, 4+5=9, !9<6, ok) in order to not 
// break these instructions
void RET6AutoHookFunction(void* function, DWORD replacedInstruction, int replacedInstructionsLen, HANDLE process) {
	// add jmps so game code -> hook code -> backup code -> game code
	char out[150];
	// allocate memory for copied instructions
	BYTE* instructionsCopy = (BYTE*)VirtualAlloc(NULL, replacedInstructionsLen + 6, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	// copy (to be) overwritten instructions
	SIZE_T bytesWritten = 0;
	ReadProcessMemory(process, (VOID*)(replacedInstruction), instructionsCopy, replacedInstructionsLen, &bytesWritten);
	if (bytesWritten != replacedInstructionsLen) {
		sprintf(out, "Failed hook 1: %x, only wrote: %i, should have written: %i", replacedInstruction, bytesWritten, replacedInstructionsLen);
		MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
	}
	// now we have copied the soon-to-be-overwritten bytes,
	// use a RET hook to jump back to the program
	RETHook6Byte(((DWORD)instructionsCopy) + replacedInstructionsLen, (void*)(replacedInstruction + replacedInstructionsLen), process);
	// write hook to process memory, create popup on success or failiure if debug

	int hookLen = 21;
	BYTE* hookCopy = (BYTE*)VirtualAlloc(NULL, hookLen + 6, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	// copy our hook code, and make execution go to the backup
	// our "hook code" in this instance is a call instruction to the real function
	SafeCALLHook21Byte((DWORD)hookCopy, function, process);
	//use RET hook to jump to copied instructions
	RETHook6Byte((DWORD)&hookCopy[hookLen], instructionsCopy, process);
	// FINALLY, add the ret hook to the program's code, going to the hook code
	// and NOP the rest of the copied bytes so it dissassembles nicely ;)
	RETHook6Byte(replacedInstruction, hookCopy, process);
	NOP(replacedInstruction + 6, replacedInstructionsLen - 6, process);
}

//NOPs len bytes at *instruction
void NOP(DWORD instruction, int len, HANDLE process) {
	BYTE nop = 0x90;
	SIZE_T bytesWritten = 0;
	for(int i=0;i<len;i++) {
		int success = WriteProcessMemory(process, (VOID*)(instruction), &nop, sizeof(nop), &bytesWritten);
	}
}