#include "stdafx.h"
#include "Utils.h"
#include "PushSourceFilters_h.h"
#include "PushSource.h"
#include "VideoSource.h"
#include "AudioSource.h"
#include "CustomAllocator.h"

//------------------------------------------------------------------------------
// PushSource filter
//------------------------------------------------------------------------------
// Filter setup data
const AMOVIESETUP_MEDIATYPE sudPinTypesDPS[] =
{
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_UYVY },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_YUY2 },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_RGB24 },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_RGB32 },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_ARGB32 },
};


const AMOVIESETUP_PIN sudOutputPinDPS =
{
	L"Video",       // Obsolete, not used.
	FALSE,          // Is this pin rendered?
	TRUE,           // Is it an output pin?
	FALSE,          // Can the filter create zero instances?
	FALSE,          // Does the filter create multiple instances?
	&CLSID_NULL,    // Obsolete.
	NULL,           // Obsolete.
	SIZEOF_ARRAY(sudPinTypesDPS),// Number of media types.
	sudPinTypesDPS  // Pointer to media types.
};

const AMOVIESETUP_FILTER sudDPS =
{
	&CLSID_PushSource,				// Filter CLSID
	L"Push Source Filter",			// String name
	MERIT_DO_NOT_USE,				// Filter merit
	1,								// Number pins
	&sudOutputPinDPS				// Pin details
};

//------------------------------------------------------------------------------
// VideoSource filter
//------------------------------------------------------------------------------
// Filter setup data
const AMOVIESETUP_MEDIATYPE sudPinTypesDVS[] =
{
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_UYVY },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_YUY2 },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_RGB32 },
	{ &MEDIATYPE_Video, &MEDIASUBTYPE_ARGB32 },
};


const AMOVIESETUP_PIN sudOutputPinDVS =
{
	L"Video",       // Obsolete, not used.
	FALSE,          // Is this pin rendered?
	TRUE,           // Is it an output pin?
	FALSE,          // Can the filter create zero instances?
	FALSE,          // Does the filter create multiple instances?
	&CLSID_NULL,    // Obsolete.
	NULL,           // Obsolete.
	SIZEOF_ARRAY(sudPinTypesDVS),// Number of media types.
	sudPinTypesDVS  // Pointer to media types.
};

const AMOVIESETUP_FILTER sudDVS =
{
	&CLSID_VideoSource,				// Filter CLSID
	L"Video Source Filter",			// String name
	MERIT_DO_NOT_USE,				// Filter merit
	1,								// Number pins
	&sudOutputPinDVS				// Pin details
};

//------------------------------------------------------------------------------
// AudioSource filter
//------------------------------------------------------------------------------
// Filter setup data
const AMOVIESETUP_MEDIATYPE sudPinTypesDAS =
{
	&MEDIATYPE_Audio,       // Major type
	&MEDIASUBTYPE_PCM      // Minor type
};


const AMOVIESETUP_PIN sudOutputPinDAS =
{
	L"Audio",      // Obsolete, not used.
	FALSE,          // Is this pin rendered?
	TRUE,           // Is it an output pin?
	FALSE,          // Can the filter create zero instances?
	FALSE,          // Does the filter create multiple instances?
	&CLSID_NULL,    // Obsolete.
	NULL,           // Obsolete.
	1,              // Number of media types.
	&sudPinTypesDAS // Pointer to media types.
};

const AMOVIESETUP_FILTER sudDAS =
{
	&CLSID_AudioSource,				// Filter CLSID
	L"Audio Source Filter",			// String name
	MERIT_DO_NOT_USE,				// Filter merit
	1,								// Number pins
	&sudOutputPinDAS				// Pin details
};

CFactoryTemplate g_Templates[] =
{
	{
		L"Push Source Filter",						// Name
		&CLSID_PushSource,							// CLSID
		CPushSource::CreateInstance,				// Method to create an instance of MyComponent
		NULL,										// Initialization function
		&sudDPS										// Set-up information (for filters)
	},
	{
		L"Push Source Custom Allocator",			// Name
		&CLSID_CustomMemAllocator,					// CLSID
		CCustomAllocator::CreateInstance,			// Method to create an instance of MyComponent
		NULL,										// Initialization function
		NULL										// Set-up information (for filters)
	},
	{
		L"Video Source Filter",						// Name
		&CLSID_VideoSource,							// CLSID
		CVideoSource::CreateInstance,				// Method to create an instance of MyComponent
		NULL,										// Initialization function
		&sudDVS										// Set-up information (for filters)
	},
	{
		L"Audio Source Filter",						// Name
		&CLSID_AudioSource,							// CLSID
		CAudioSource::CreateInstance,				// Method to create an instance of MyComponent
		NULL,										// Initialization function
		&sudDAS										// Set-up information (for filters)
	},
};

int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

STDAPI DllRegisterServer()
{
	HRESULT hr = AMovieDllRegisterServer2(TRUE);

	// register type library
	WCHAR achFileName[MAX_PATH];
	GetModuleFileNameW(g_hInst, achFileName, MAX_PATH);
	ITypeLib* pTypeLib;
	LoadTypeLib(achFileName, &pTypeLib);
	if (pTypeLib)
	{
		RegisterTypeLib(pTypeLib, achFileName, NULL);
		pTypeLib->Release();
	}

	return hr;
}

STDAPI DllUnregisterServer()
{
	HRESULT hr = AMovieDllRegisterServer2(FALSE);

	// unregister type library
	WCHAR achFileName[MAX_PATH];
	GetModuleFileNameW(g_hInst, achFileName, MAX_PATH);
	ITypeLib* pTypeLib;
	LoadTypeLib(achFileName, &pTypeLib);

	if (pTypeLib)
	{
		TLIBATTR* ptla;
		hr = pTypeLib->GetLibAttr(&ptla);
		if (SUCCEEDED(hr))
		{
			hr = UnRegisterTypeLib(ptla->guid, ptla->wMajorVerNum, ptla->wMinorVerNum, ptla->lcid, ptla->syskind);
			pTypeLib->ReleaseTLibAttr(ptla);
		}
		pTypeLib->Release();
	}

	return hr;
}

extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}

