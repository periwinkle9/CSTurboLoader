#include "common.h"
#include <ShlObj.h>
#include <string>

static HMODULE getRealVersionDLL()
{
    static HMODULE realVersionDLL = NULL;

    if (realVersionDLL == NULL)
    {
        // Get path to system32
        PWSTR system32;
        if (SHGetKnownFolderPath(FOLDERID_SystemX86, 0, NULL, &system32) == S_OK)
        {
            std::wstring versionDLLPath{system32};
            versionDLLPath += L"\\version.dll";

            realVersionDLL = LoadLibraryW(versionDLLPath.c_str());
            if (realVersionDLL)
                OutputDebugStringW((L"CS Turbo Mod: Loaded version.dll from " + versionDLLPath).c_str());
            else
                OutputDebugStringW((L"CS Turbo Mod: Failed to load version.dll! Path: " + versionDLLPath).c_str());
        }
        else
            OutputDebugStringW(L"CS Turbo Mod: Failed to get system directory");
        CoTaskMemFree(system32);
    }

    return realVersionDLL;
}

extern "C" {

// Function headers copy/pasted from the corresponding MSVC headers

#pragma comment(linker, "/export:GetFileVersionInfoSizeA=_Real_GetFileVersionInfoSizeA@8")
DWORD
APIENTRY
Real_GetFileVersionInfoSizeA(
    _In_        LPCSTR lptstrFilename,
    _Out_opt_ LPDWORD lpdwHandle
)
{
    using Func = decltype(&Real_GetFileVersionInfoSizeA);
    static Func addr = nullptr;

    if (!addr)
        addr = (Func)GetProcAddress(getRealVersionDLL(), "GetFileVersionInfoSizeA");
    return addr(lptstrFilename, lpdwHandle);
}

#pragma comment(linker, "/export:GetFileVersionInfoA=_Real_GetFileVersionInfoA@16")
BOOL
APIENTRY
Real_GetFileVersionInfoA(
    _In_                LPCSTR lptstrFilename,
    _Reserved_          DWORD dwHandle,
    _In_                DWORD dwLen,
    _Out_writes_bytes_(dwLen) LPVOID lpData
)
{
    using Func = decltype(&Real_GetFileVersionInfoA);
    static Func addr = nullptr;

    if (!addr)
        addr = (Func)GetProcAddress(getRealVersionDLL(), "GetFileVersionInfoA");

    return addr(lptstrFilename, dwHandle, dwLen, lpData);
}

#pragma comment(linker, "/export:VerQueryValueA=_Real_VerQueryValueA@16")
BOOL
APIENTRY
Real_VerQueryValueA(
    _In_ LPCVOID pBlock,
    _In_ LPCSTR lpSubBlock,
    _Outptr_result_buffer_(_Inexpressible_("buffer can be PWSTR or DWORD*")) LPVOID* lplpBuffer,
    _Out_ PUINT puLen
)
{
    using Func = decltype(&Real_VerQueryValueA);
    static Func addr = nullptr;

    if (!addr)
        addr = (Func)GetProcAddress(getRealVersionDLL(), "VerQueryValueA");

    return addr(pBlock, lpSubBlock, lplpBuffer, puLen);
}

}
