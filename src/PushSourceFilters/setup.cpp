#include "stdafx.h"

CFactoryTemplate g_Templates[1];
int g_cTemplates = 0;

STDAPI DllRegisterServer()
{
	return S_OK;
}

STDAPI DllUnregisterServer()
{
	return S_OK;
}

extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}

