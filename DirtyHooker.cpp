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
void RETHook6Byte(DWORD instruction, void* function, HANDLE process) {
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

//replaces instruction with  call &function (near, absolute)
//example usage: CALLHook6Byte(0x12345678, functionName);
void CALLHook6Byte(DWORD instruction, void* function, HANDLE process) {
	BYTE hook[6] = {0xFF, 0x15, 0x00, 0x00, 0x00, 0x00};
	//write function address to PUSH instruction, bytes are in reverse order
	hook[1] = ((DWORD)function>>0)&0xFF;
	hook[2] = ((DWORD)function>>8)&0xFF;
	hook[3] = ((DWORD)function>>16)&0xFF;
	hook[4] = ((DWORD)function>>24)&0xFF;
	// write hook to process memory, create popup on success or failiure if debug
	char message[50];
	SIZE_T bytesWritten = 0;
	int success = WriteProcessMemory(process, (VOID*)(0x0025DBB8+0x00400000), &hook, sizeof(hook), &bytesWritten);
	char out[50];
	if(DEBUG){
		sprintf(out,"Test %i",bytesWritten);
		MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
	}
}
/*
//JMPAutoHook copies bytes of replaced instructions to location, and adds a JMP instruction pointing to next instruction after replaced ones
//JMPAutoHook replaces bytes to push location of hook, and location of copied replaced instructions, calls to helper
//Helper takes address of hook & address of copied bytes, calls hook, jmps to copied bytes

//allocates block 
void JMPAutoHookASMBlockHelper(void* address, void* function){
	//hook asm
	_asm{
hookStart:
		//call hook
		call eax;
		//jmp to copied instructions
		jmp ebx;
	}
}

//replaces instruction with  call &function (near, absolute)
//example usage: CALLHook6Byte(0x12345678, functionName);
void JMPAutoHookASMBlock(void* hook, DWORD replacedInstruction, int replacedInstructionsLen, HANDLE process) {
	char out[50];
	//get offset of the helper's ASM
	void *hStart, *hEnd;
	__asm{ mov [hStart],offset hookStart };
	__asm{ mov [hEnd],offset hookEnd };
	//allocate memory for copied instructions
	BYTE* instructionsCopy = (BYTE*)malloc(replacedInstructionsLen+6);
	//copy instructions
	SIZE_T bytesWritten = 0;
	ReadProcessMemory(process, (VOID*)(replacedInstruction), &instructionsCopy, replacedInstructionsLen, &bytesWritten);
	if(bytesWritten != replacedInstructionsLen){
		sprintf(out,"Failed hook: %x, only wrote: %i, should have written: %i",replacedInstruction,bytesWritten,replacedInstructionsLen);
		MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
	}
	//now we have copied the bytes, 
	//use a RET hook to jump back to the program
	RETHook6Byte((DWORD)&instructionsCopy[replacedInstructionsLen],(void*)(replacedInstruction+replacedInstructionsLen));
	// write hook to process memory, create popup on success or failiure if debug
	int success = WriteProcessMemory(process, (VOID*)(0x0025DBB8+0x00400000), &hook, sizeof(hook), &bytesWritten);
	if(bytesWritten != replacedInstructionsLen){
		sprintf(out,"Failed hook: %x, only wrote: %i, should have written: %i",replacedInstruction,bytesWritten,replacedInstructionsLen);
		MessageBox(NULL, out, "test", MB_OK + MB_ICONINFORMATION);
	}
}
*/
//NOPs len bytes at *instruction
void NOP(DWORD instruction, int len, HANDLE process) {
	BYTE nop = 0x90;
	SIZE_T bytesWritten = 0;
	for(int i=0;i<len;i++) {
		int success = WriteProcessMemory(process, (VOID*)(instruction), &nop, sizeof(nop), &bytesWritten);
	}
}