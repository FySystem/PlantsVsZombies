#include "pch.h"
#include "framework.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Global::get()->hDllInst = hModule;
        DisableThreadLibraryCalls(Global::get()->hDllInst);

        //绘制菜单
        Menu::get()->Execute();
        break;
    case DLL_PROCESS_DETACH:

        break;
    }
    return TRUE;
}

