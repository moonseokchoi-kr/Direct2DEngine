#pragma once
class CDevice
	:public CSingleton<CDevice>
{
	SINGLE(CDevice);
public:
	HRESULT Init(HWND _mainHwnd, Vec2 _vResoultion);
	HRESULT CreateSwapChain();
	HRESULT CreateView();
	HRESULT CreateViewport();
private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	

	IDXGISwapChain* m_swapChain;
	ID3D11Texture2D* m_RTTex;
	ID3D11Texture2D* m_DSTex;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilState* m_dst;

	array<float, 4> m_clearColors;
	Vec2 m_resolution;
	HWND m_hWnd;
	bool m_bWindow;

};

