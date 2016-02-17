#pragma once

#include "PushSourceFilters_h.h"

//------------------------------------------------------------------------------
// CPushPin class
//------------------------------------------------------------------------------
// This class defines the pin on our filter
//
class CPushPin : public CSourceStream
						, public IAMStreamConfig
						, public IAMBufferNegotiation
{
public:
    CPushPin(TCHAR* pObjectName, HRESULT* phr, CSource* pFilter, LPCWSTR pName);
    ~CPushPin();

	DECLARE_IUNKNOWN;

    // Base class overrides
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void** ppv);
	virtual HRESULT InitAllocator(IMemAllocator** ppAllocator);
	virtual HRESULT DecideAllocator(IMemInputPin* pPin, IMemAllocator** ppAlloc);

    // Override the version that offers exactly one media type
    virtual HRESULT GetMediaType(CMediaType* pMediaType);
    virtual HRESULT DecideBufferSize(IMemAllocator* pAlloc, ALLOCATOR_PROPERTIES* pRequest);
    virtual HRESULT FillBuffer(IMediaSample* pSample);
	virtual HRESULT OnThreadStartPlay(void);
    
    // Quality control
	// Not implemented because we aren't going in real time.
	// If the file-writing filter slows the graph down, we just do nothing, which means
	// wait until we're unblocked. No frames are ever dropped.
    STDMETHODIMP Notify(IBaseFilter* pSelf, Quality q)
    {
        return E_FAIL;
    }

	// IAMStreamConfig interface
    STDMETHODIMP GetFormat(AM_MEDIA_TYPE** ppamt);
    STDMETHODIMP GetNumberOfCapabilities(int* piCount, int* piSize);
    STDMETHODIMP GetStreamCaps(int iIndex, AM_MEDIA_TYPE** ppamt, BYTE* pSCC);
    STDMETHODIMP SetFormat(AM_MEDIA_TYPE* pamt);

	// IAMBufferNegotiation interface
	STDMETHODIMP SuggestAllocatorProperties(const ALLOCATOR_PROPERTIES* pprop);
	STDMETHODIMP GetAllocatorProperties(ALLOCATOR_PROPERTIES* pprop);

protected:
    int m_iFrameNumber;					// number of frames delivered
    REFERENCE_TIME m_rtFrameLength;		// frame duration in 100ns units
    REFERENCE_TIME m_rtNextValidStart;	// used to guarantee that sample timestamps do not overlap

    CCritSec m_cSharedState;            // Protects our internal state

	CMediaType m_MediaType;	// media type of the desired connection

	// IAMBufferNegotiation interface support
	ALLOCATOR_PROPERTIES m_allocProp;
};

//------------------------------------------------------------------------------
// CPushSource
//------------------------------------------------------------------------------
// This class defines our filter
//
class CPushSource : public CSource
						, public IAMFilterMiscFlags
{

private:
    // Constructor is private because you have to use CreateInstance
    CPushSource(IUnknown* pUnk, HRESULT* phr);
    ~CPushSource();

	DECLARE_IUNKNOWN;

    // Base class overrides
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void** ppv);

	// IMediaFilter interface override
	STDMETHODIMP GetState(DWORD dwMilliSecsTimeout, FILTER_STATE* pState);

	// IAMFilterMiscFlags interface
	ULONG STDMETHODCALLTYPE GetMiscFlags(void) {return AM_FILTER_MISC_FLAGS_IS_SOURCE;}

    CPushPin* m_pPin;

public:
    static CUnknown* WINAPI CreateInstance(IUnknown* pUnk, HRESULT* phr);  

};

