#pragma once

#include <Windows.h>
#include <msctf.h>
#include <atlbase.h> // °üº¬ CComPtr

class TSFManager : public ITfEditSession {
public:
    TSFManager();
    ~TSFManager();

    void InsertTextAtCaret(const wchar_t* text);

    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObj) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // ITfEditSession methods
    STDMETHODIMP DoEditSession(TfEditCookie ec) override;

private:
    void InitTSF();
    void CleanupTSF();

    ITfThreadMgr* pThreadMgr;
    ITfDocumentMgr* pDocMgr;
    ITfContext* pContext;
    TfClientId tfClientId;
    TfEditCookie tfEditCookie;
    const wchar_t* textToInsert; // Text to insert

    HRESULT StartEditSession();

    LONG _refCount; // Reference count for IUnknown
};