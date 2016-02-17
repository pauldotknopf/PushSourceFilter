
#pragma once

#include "PushSource.h"

//------------------------------------------------------------------------------
// CVideoSourcePin class
//------------------------------------------------------------------------------
// This class defines the video pin on our filter
//
class CVideoSourcePin : public CPushPin
{
public:
    CVideoSourcePin(HRESULT* phr, CSource* pFilter);

    // Override the version that offers exactly one media type
    virtual HRESULT DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* pRequest);
    virtual HRESULT FillBuffer(IMediaSample* pSample);
    
	// IAMStreamConfig interface
    STDMETHODIMP GetFormat(AM_MEDIA_TYPE** ppamt);
    STDMETHODIMP SetFormat(AM_MEDIA_TYPE* pamt);
};

//------------------------------------------------------------------------------
// CVideoSource
//------------------------------------------------------------------------------
// This class defines our filter
//
class CVideoSource : public CSource
//						, public IAMFilterMiscFlags
{

private:
    // Constructor is private because you have to use CreateInstance
    CVideoSource(IUnknown* pUnk, HRESULT* phr);
    ~CVideoSource();

	DECLARE_IUNKNOWN;

    // Base class overrides
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void** ppv);

	// IMediaFilter interface override
	STDMETHODIMP GetState(DWORD dwMilliSecsTimeout, FILTER_STATE* pState);

	// IAMFilterMiscFlags interface
//	ULONG STDMETHODCALLTYPE GetMiscFlags(void) {return AM_FILTER_MISC_FLAGS_IS_SOURCE;}

    CVideoSourcePin* m_pPin;

public:
    static CUnknown* WINAPI CreateInstance(IUnknown* pUnk, HRESULT* phr);  

};

