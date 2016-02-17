#include "stdafx.h"
#include "streams.h"

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
	/*switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;*/

	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}

