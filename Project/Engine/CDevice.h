#pragma once

class CConstBuffer;
class CDevice
	:public CSingleton<CDevice>
{
	SINGLE(CDevice);
public:
	HRESULT Init(HWND _mainHwnd, Vec2 _vResoultion);
public:
	void ClearTarget();
	void Present();
	HRESULT OnReSize(Vec2 _resolution);
public:
	ID3D11Device* GetDevice() { return m_device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_context.Get(); }
	float GetAspectRatio() { return (float)m_resolution.x / m_resolution.y; }
	CConstBuffer* GetConstBuffer(CB_TYPE _type) { return m_constBuffers[(UINT)_type]; }


	bool IsWindow() { return m_window; }
	void SetWindow(bool _b) { m_window = _b; }
private:
	HRESULT CreateSwapChain();
	HRESULT CreateView();
	HRESULT CreateConstBuffer();

	HRESULT CreateSampler();
private:
	ComPtr<IDXGIAdapter> m_adapter;
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	

	array<CConstBuffer*, (UINT)CB_TYPE::END> m_constBuffers;
	array<ComPtr<ID3D11SamplerState>, 2> m_samplers;

	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11Texture2D> m_DSVBuffer;
	ComPtr<ID3D11RenderTargetView> m_RTV;
	ComPtr<ID3D11DepthStencilView> m_DSV;
	D3D11_VIEWPORT	m_VP;


	Vec2 m_resolution;
	HWND m_hWnd;
	bool m_window;
};

