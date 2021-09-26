#include "pch.h"
#include "CDevice.h"

CDevice::CDevice()
	:m_hWnd(nullptr)
	,m_device(nullptr)
	,m_context(nullptr)
	,m_bWindow(true)
{

}
CDevice::~CDevice()
{
}
HRESULT CDevice::Init(HWND _mainHwnd, Vec2 _vResoultion)
{
	m_hWnd = _mainHwnd;
	m_resolution = _vResoultion;
	
	UINT deviceFlag = 0;
#ifdef _DEBUG
	deviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlag, 0, 0, D3D11_SDK_VERSION, &m_device, &featureLevel, &m_context)))
	{
		MessageBox(nullptr, L"Direct 초기화 실패", L"Engine 초기화 오류", MB_OK);
		return E_FAIL;
	}
	
	//그래픽카드가 Direct를 지원하는지 확인
	//4x MSAA(multi samples anti-aliasing)지원 확인
	//11부터는 이걸이용해 지원이 되는지 안되는지 확인가능(무조건 4x MSAA를 지원하기때문)
	UINT multiSmapleLevel = 0;
	m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &multiSmapleLevel);
	if (0 == multiSmapleLevel)
	{
		MessageBox(nullptr, L"하드웨어가 4xMSAA를 지원하지 않습니다", L"Engine초기화 오류", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"스왑체인 생성 실패", L"Engine초기화 오류", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateView()))
	{
		MessageBox(nullptr, L"뷰 생성실패", L"Engine초기화 오류", MB_OK);
		return E_FAIL;
	}

	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	vp.Width = (UINT)m_resolution.x;
	vp.Height = (UINT)m_resolution.y;

	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	m_context->RSSetViewports(1, &vp);

	return S_OK;
}

void CDevice::ClearTarget()
{
	float color[4] = { 0.4f,0.4f,0.4f,1.0f };
	m_context->ClearRenderTargetView(m_RTV.Get(), color);
	m_context->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f ,0);
}

void CDevice::Present()
{
	m_swapChain.Get()->Present(0, 0);
}

HRESULT CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};

	desc.BufferDesc.Width = m_resolution.x;
	desc.BufferDesc.Height = m_resolution.y;

	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//추후에 고정프레임 변경가능하도록 하기
	//60
	//120
	//144
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;

	desc.BufferCount = 1;

	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	desc.Windowed = m_bWindow;
	desc.OutputWindow = m_hWnd;
	desc.Flags = 0;


	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory* dxgiFactory = nullptr;

	m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	if (FAILED(dxgiFactory->CreateSwapChain(m_device.Get(), &desc, m_swapChain.GetAddressOf())))
	{
		return E_FAIL;
	}

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	return S_OK;
}

HRESULT CDevice::CreateView()
{
	//RTV
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	if (FAILED(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_RTV.GetAddressOf())))
	{
		return E_FAIL;
	}
	//DRV
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = (UINT)m_resolution.x;
	desc.Height = (UINT)m_resolution.y;

	desc.MipLevels = 1;
	desc.ArraySize = 1;

	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (FAILED(m_device->CreateTexture2D(&desc, 0, m_DSVTex.GetAddressOf())))
	{
		return E_FAIL;
	}
	if (FAILED(m_device->CreateDepthStencilView(m_DSVTex.Get(), nullptr, m_DSV.GetAddressOf())))
	{
		return E_FAIL;
	}

	m_context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());

	return S_OK;
}
