#pragma once
#include <Windows.h>
#define DEBUG false

struct HookAddr {
	DWORD startAddress;
	DWORD endAddress;
};

void MovAddrReg6Byte(DWORD instruction, char* const reg, void* pValue, HANDLE process = (HANDLE)-1);
void RETHook6Byte(DWORD instruction, void* function, HANDLE process = (HANDLE)-1);
void CALLHook5Byte(DWORD instruction, void* function, HANDLE process = (HANDLE)-1);
void NOP(DWORD instruction, int len, HANDLE process = (HANDLE)-1);
void RET6AutoHookASMBlock(HookAddr hook, DWORD replacedInstruction, int replacedInstructionsLen, HANDLE process);
void RET6AutoHookFunction(void* function, DWORD replacedInstruction, int replacedInstructionsLen, HANDLE process);
void RET6CALL5AutoHookNaked(void* hook, DWORD replacedInstruction, int replacedInstructionsLen, HANDLE process);