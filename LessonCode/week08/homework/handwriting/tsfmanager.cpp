#include "tsfmanager.h"

TSFManager::TSFManager() : pThreadMgr(nullptr), pDocMgr(nullptr), pContext(nullptr), tfClientId(TF_CLIENTID_NULL), tfEditCookie(0), textToInsert(nullptr), _refCount(1) {
    CoInitialize(NULL);
    InitTSF();
}

TSFManager::~TSFManager() {
    CleanupTSF();
    CoUninitialize();
}

void TSFManager::InitTSF() {
    HRESULT hr = CoCreateInstance(CLSID_TF_ThreadMgr, nullptr, CLSCTX_INPROC_SERVER, IID_ITfThreadMgr, (void**)&pThreadMgr);
    if (SUCCEEDED(hr)) {
        hr = pThreadMgr->Activate(&tfClientId);
        if (SUCCEEDED(hr)) {
            hr = pThreadMgr->CreateDocumentMgr(&pDocMgr);
            if (SUCCEEDED(hr)) {
                hr = pDocMgr->CreateContext(tfClientId, 0, nullptr, &pContext, &tfEditCookie);
                if (SUCCEEDED(hr)) {
                    pDocMgr->Push(pContext);
                }
            }
        }
    }
}

void TSFManager::CleanupTSF() {
    if (pDocMgr) {
        pDocMgr->Pop(TF_POPF_ALL);
        pDocMgr->Release();
    }
    if (pContext) {
        pContext->Release();
    }
    if (pThreadMgr) {
        pThreadMgr->Deactivate();
        pThreadMgr->Release();
    }
}

void TSFManager::InsertTextAtCaret(const wchar_t* text) {
    textToInsert = text;
    StartEditSession();
}

HRESULT TSFManager::StartEditSession() {
    if (pContext == nullptr)
        return E_FAIL;

    HRESULT hr = S_OK;
    hr = pContext->RequestEditSession(tfClientId, this, TF_ES_ASYNCDONTCARE | TF_ES_READWRITE, &hr);

    // 如果需要检查或处理异步调用的结果，可以在回调中进行
    // 这里我们只是启动编辑会话，并不等待结果
    return hr;
}

STDMETHODIMP TSFManager::DoEditSession(TfEditCookie ec) {
    CComPtr<ITfInsertAtSelection> pInsertAtSelection;
    CComPtr<ITfRange> pRange;
    HRESULT hr = pContext->QueryInterface(IID_ITfInsertAtSelection, (void**)&pInsertAtSelection);
    if (SUCCEEDED(hr)) {
        DWORD textLength = static_cast<DWORD>(wcslen(textToInsert));
        hr = pInsertAtSelection->InsertTextAtSelection(ec, TF_IAS_NOQUERY, textToInsert, textLength, &pRange);
        if (SUCCEEDED(hr)) {
            // 更新光标位置
            TF_SELECTION tfSelection;
            tfSelection.range = pRange; // 设置范围
            tfSelection.style.ase = TF_AE_START;
            tfSelection.style.fInterimChar = FALSE;
            hr = pContext->SetSelection(ec, 1, &tfSelection); // 使用 TF_SELECTION 结构
        }
    }
    return S_OK;
}

// IUnknown methods
STDMETHODIMP TSFManager::QueryInterface(REFIID riid, void** ppvObj) {
    if (ppvObj == nullptr)
        return E_POINTER;

    if (riid == IID_IUnknown || riid == IID_ITfEditSession) {
        *ppvObj = static_cast<ITfEditSession*>(this);
    }
    else {
        *ppvObj = nullptr;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG) TSFManager::AddRef() {
    return InterlockedIncrement(&_refCount);
}

STDMETHODIMP_(ULONG) TSFManager::Release() {
    ULONG ulRefCount = InterlockedDecrement(&_refCount);
    if (ulRefCount == 0) {
        delete this;
    }
    return ulRefCount;
}