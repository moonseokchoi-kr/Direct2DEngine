#include "pch.h"
#include "CCore.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CDevice.h"
#include "CSceneManager.h"

#include "temp.h"

CCore::CCore()
    :hWnd_(nullptr)
{

}

CCore::~CCore()
{
}

HRESULT CCore::Init(HWND hWnd, UINT x, UINT y)
{
    hWnd_ = hWnd;
    ChangeWindowSize(x, y);

    if (FAILED(CDevice::GetInst()->Init(hWnd_, Vec2(static_cast<float>(x), static_cast<float>(y)))))
    {
        return E_FAIL;
    }

    CTimeManager::GetInst()->Init();
    CPathManager::GetInst()->Init();
    CKeyManager::GetInst()->Init();
    CResourceManager::GetInst()->Init();
    CSceneManager::GetInst()->Init();
    return S_OK;
}

void CCore::Progress()
{
    CTimeManager::GetInst()->Update();
    CKeyManager::GetInst()->Update();
    CSceneManager::GetInst()->Progress();
}

void CCore::ChangeWindowSize(UINT x, UINT y)
{
    RECT rt = { 0,0,static_cast<LONG>(x),static_cast<LONG>(y) };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

    SetWindowPos(hWnd_, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}
