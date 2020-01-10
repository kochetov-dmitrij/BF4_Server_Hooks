#include "ASM_Hooks.h"

/*
	Battlefield 4 server build 179665/R63 hooks
	https://github.com/kochetov-dmitrij/BF4_Server_Hooks
*/

// Initialize the hooks
DWORD WINAPI InitThread(LPVOID lpThreadParameter)
{
	ASM_Hooks::enableNolevolution(reinterpret_cast<LPVOID>(OFFSET_healthSetterInstruction), reinterpret_cast<LPVOID>(OFFSET_healthSetterCave));
	return TRUE;
}

// Library entry point 
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) CloseHandle(CreateThread(0, 0, InitThread, hModule, 0, 0));
	return TRUE;
}
