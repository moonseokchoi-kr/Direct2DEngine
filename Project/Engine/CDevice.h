#pragma once

class CConstBuffer;
class CDevice
	:public CSingleton<CDevice>
{
	SINGLE(CDevice);
public:
	HRESULT Init(HWND mainHWnd, Vec2 resoultion);
public:
	void ClearTarget();
	void Present();
	HRESULT OnReSize(Vec2 _resolution);
public:
	ID3D11Device* GetDevice() { return device_.Get(); }
	ID3D11DeviceContext* GetContext() { return context_.Get(); }
	
	float GetAspectRatio() { return resolution_.x / resolution_.y; }
	Vec2 GetResolution() { return resolution_; }
	
	CConstBuffer* GetConstBuffer(CB_TYPE type) { return const_buffer_array_[static_cast<UINT>(type)]; }
	
	ComPtr<ID3D11RasterizerState> GetRasterizerState(RASTERIZER_TYPE type) { return rasterizer_state_array_[static_cast<UINT>(type)]; }
	ComPtr<ID3D11BlendState> GetBlendState(BLEND_TYPE type) { return blend_state_array_[static_cast<UINT>(type)]; }


	bool IsWindow() { return window_; }
	void SetWindow(bool b) { window_ = b; }
private:
	HRESULT CreateSwapChain();
	HRESULT CreateView();
	HRESULT CreateConstBuffer();
	HRESULT CreateBlendState();
	HRESULT CreateRasterizerState();
	HRESULT CreateDepthStencilState();
	HRESULT CreateSampler();
private:
	ComPtr<IDXGIAdapter> adapter_;
	ComPtr<ID3D11Device> device_;
	ComPtr<ID3D11DeviceContext> context_;
	

	array<CConstBuffer*, static_cast<UINT>(CB_TYPE::END)> const_buffer_array_;
	array<ComPtr<ID3D11DepthStencilState>, static_cast<UINT>(DEPTH_STENCIL_TYPE::END)> depth_stencil_state_array_;
	array<ComPtr<ID3D11SamplerState>, 2> sampler_state_array_;
	array<ComPtr<ID3D11BlendState>, static_cast<UINT>(BLEND_TYPE::END)> blend_state_array_;
	array<ComPtr<ID3D11RasterizerState>, static_cast<UINT>(RASTERIZER_TYPE::END)> rasterizer_state_array_;



	ComPtr<IDXGISwapChain> swap_chain_;
	ComPtr<ID3D11Texture2D> depth_stencil_buffer_;
	ComPtr<ID3D11RenderTargetView> render_target_view_;
	ComPtr<ID3D11DepthStencilView> depth_stencil_view_;
	D3D11_VIEWPORT	view_port_;


	Vec2 resolution_;
	HWND hWnd_;
	bool window_;
};

