#pragma once

#include "PushSource.h"

//------------------------------------------------------------------------------
// CAudioSourcePin class
//------------------------------------------------------------------------------
// This class defines the pin on our filter
//
class CAudioSourcePin : public CPushPin
{
public:
    CAudioSourcePin(HRESULT* phr, CSource* pFilter);

    // Override the version that offers exactly one media type
    virtual HRESULT DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* pRequest);
    virtual HRESULT FillBuffer(IMediaSample* pSample);
	virtual HRESULT OnThreadStartPlay(void);

	// IAMStreamConfig interface
    STDMETHODIMP GetFormat(AM_MEDIA_TYPE** ppamt);
    STDMETHODIMP SetFormat(AM_MEDIA_TYPE* pamt);

private:
    REFERENCE_TIME m_rtMediaTime;
};

//------------------------------------------------------------------------------
// CAudioSource
//------------------------------------------------------------------------------
// This class defines our filter
//
class CAudioSource : public CSource
						, public IAMFilterMiscFlags
{

private:
    // Constructor is private because you have to use CreateInstance
    CAudioSource(IUnknown* pUnk, HRESULT* phr);
    ~CAudioSource();

	DECLARE_IUNKNOWN;

    // Base class overrides
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void** ppv);

	// IMediaFilter interface override
	STDMETHODIMP GetState(DWORD dwMilliSecsTimeout, FILTER_STATE* pState);

	// IAMFilterMiscFlags interface
	ULONG STDMETHODCALLTYPE GetMiscFlags(void) {return AM_FILTER_MISC_FLAGS_IS_SOURCE;}

    CAudioSourcePin* m_pPin;

public:
    static CUnknown* WINAPI CreateInstance(IUnknown* pUnk, HRESULT* phr);  

};

