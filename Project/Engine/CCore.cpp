#include "pch.h"
#include "CCore.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CDevice.h"

#include "temp.h"
CCore::CCore()
    :m_hWnd(nullptr)
{

}

CCore::~CCore()
{
}

HRESULT CCore::Init(HWND _hwnd, UINT _x, UINT _y)
{
    m_hWnd = _hwnd;
    ChangeWindowSize(_x, _y);

    if (FAILED(CDevice::GetInst()->Init(m_hWnd, Vec2((float)_x, (float)_y))))
    {
        return E_FAIL;
    }

    CTimeManager::GetInst()->Init();
    CPathManager::GetInst()->Init();
    CKeyManager::GetInst()->Init();
    CResourceManager::GetInst()->Init();
    
    return S_OK;
}

void CCore::Progress()
{
    CTimeManager::GetInst()->Update();
    CKeyManager::GetInst()->Update();
    Update();
    Render();
}

void CCore::ChangeWindowSize(UINT _x, UINT _y)
{
    RECT rt = { 0,0,(LONG)_x,(LONG)_y };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

    SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}
