/*
* Microsoft Detours Educational Example by ethical.blue
* Created with Microsoft Visual Studio 2022
*/

#include <Windows.h>
#include <cstdio>
#include <clocale>
#include "..\detours\detours.h"

#pragma comment(lib, "..\\x64\\Debug\\detours.lib")

VOID(WINAPI* pSleep)(DWORD) = Sleep;
VOID WINAPI MySleep(DWORD dwMilliseconds);

VOID WINAPI MySleep(DWORD dwMilliseconds)
{
    FILE* pLogFile;

    setlocale(LC_CTYPE, "");

    _wfopen_s(&pLogFile, L"log.txt", L"a+");

    if (pLogFile != NULL)
    {
        fwprintf(pLogFile, L"Monitorowany program zasn¹³ (funkcja Sleep) na %i milisekund.", dwMilliseconds);

        fclose(pLogFile);
    }

    return pSleep(dwMilliseconds);
}

extern "C" __declspec(dllexport) void nothing(void) {
    return;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (DetourIsHelperProcess())
        return TRUE;

    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DetourRestoreAfterWith();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourAttach(&(PVOID&)pSleep, MySleep);
        DetourTransactionCommit();
    }
    else if (fdwReason == DLL_PROCESS_DETACH)
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourDetach(&(PVOID&)pSleep, MySleep);
        DetourTransactionCommit();
    }

    return TRUE;
}