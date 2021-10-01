#pragma once
class CCore
	:public CSingleton<CCore>
{
	SINGLE(CCore);

public:
	HRESULT Init(HWND _hwnd, UINT _x, UINT _y);

	void Progress();
public:
	void ChangeWindowSize(UINT _x, UINT _y);

public:
	HWND GetMainHwnd() { return m_hWnd; }

private:
	HWND m_hWnd;

};

