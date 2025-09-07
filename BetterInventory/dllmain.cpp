#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Hooks.h"
static DWORD __stdcall MainThread(LPVOID lpParam) {
    MH_Initialize();
    g_Hooks.init();
    while (true) {
        Sleep(100);
    }
    return 0x0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);
        DisableThreadLibraryCalls(hModule);
    }
    return TRUE;
}

