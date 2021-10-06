#pragma once
class CCore
	:public CSingleton<CCore>
{
	SINGLE(CCore);

public:
	HRESULT Init(HWND hWnd, UINT x, UINT y);

	void Progress();
public:
	void ChangeWindowSize(UINT x, UINT y);

public:
	HWND GetMainHwnd() { return hWnd_; }

private:
	HWND hWnd_;

};

