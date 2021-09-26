#pragma once
class CDevice
	:public CSingleton<CDevice>
{
	SINGLE(CDevice);
public:
	HRESULT Init(HWND _mainHwnd, Vec2 _vResoultion);
public:
	void ClearTarget();
	void Present();

public:
	ID3D11Device* GetDevice() { return m_device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_context.Get(); }
private:
	HRESULT CreateSwapChain();
	HRESULT CreateView();
private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	

	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11Texture2D> m_RTVTex;
	ComPtr<ID3D11Texture2D> m_DSVTex;
	ComPtr<ID3D11RenderTargetView> m_RTV;
	ComPtr<ID3D11DepthStencilView> m_DSV;

	Vec2 m_resolution;
	HWND m_hWnd;
	bool m_bWindow;

};

