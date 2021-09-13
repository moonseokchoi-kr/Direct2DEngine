#include "pch.h"
#include "CCore.h"
#include "CTimeManager.h"
#include "CKeyManager.h"

#include "CPathManager.h"

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

    if (FAILED())
    {
        return E_FAIL;
    }

    CTimeManager::GetInst()->Init();
    CPathManager::GetInst()->Init();
    CKeyManager::GetInst()->Init();
    return E_NOTIMPL;
}

void CCore::Progress()
{
    CTimeManager::GetInst()->Update();
    CKeyManager::GetInst()->Update();
}

void CCore::ChangeWindowSize(UINT _x, UINT _y)
{
}
