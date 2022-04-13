/*
* Microsoft Detours Educational Example by ethical.blue
* Created with Microsoft Visual Studio 2022
*/

#include <Windows.h>
#include "..\detours\detours.h"

#pragma comment(lib, "..\\x64\\Debug\\detours.lib")

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    ZeroMemory(&pi, sizeof(pi));

    BOOL injected = FALSE;

    injected = DetourCreateProcessWithDllExA(
        "prog1.exe", NULL, NULL, NULL, TRUE,
        CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED,
        NULL, NULL, &si, &pi, "library.dll", NULL);

    if (injected == TRUE)
    {
        MessageBox(0, L"Biblioteka DLL zosta³a wstrzykniêta w proces.",
            L"Informacja", MB_OK + MB_ICONINFORMATION);
    }
    else
    {
        MessageBox(0, L"Nie uda³o siê wstrzykn¹æ biblioteki DLL.",
            L"B³¹d", MB_OK + MB_ICONERROR);
    }

    ResumeThread(pi.hThread);

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(&si);
    CloseHandle(&pi);

    return EXIT_SUCCESS;
}