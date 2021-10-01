#include "pch.h"
#include "CDevice.h"
#include "CConstBuffer.h"
CDevice::CDevice()
	:m_hWnd(nullptr)
	,m_device(nullptr)
	,m_context(nullptr)
	,m_window(true)
	, m_VP{}
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

	m_VP.TopLeftX = 0;
	m_VP.TopLeftY = 0;

	m_VP.Width = m_resolution.x;
	m_VP.Height = m_resolution.y;

	m_VP.MinDepth = 0;
	m_VP.MaxDepth = 1;

	m_context->RSSetViewports(1, &m_VP);

	if (FAILED(CreateSampler()))
	{
		MessageBox(nullptr, L"Sampler 생성 실패", L"Engine초기화 오류", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateConstBuffer()))
	{
		MessageBox(nullptr, L"상수버퍼 초기화 실패", L"Engine초기화 오류", MB_OK);
		return E_FAIL;
	}

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

HRESULT CDevice::OnReSize(Vec2 _resolution)
{
	m_resolution = _resolution;

	assert(m_context);
	assert(m_device);
	assert(m_swapChain);
	//comptr로 선언되면 공유횟수가 0가 되어야 릴리즈됨, 근데 만약 강제로 릴리즈시키면 참조하고 있던애들이 붕뜨게됨, 문제는 참조하고 있던 애들이 누군지 모름
	//강제로 부서도 되는지 모르겠음
	m_RTV->Release();
	m_RTV = nullptr;
	m_DSV->Release();
	m_DSV = nullptr;
	m_DSVBuffer->Release();
	m_DSVBuffer = nullptr;
	

	HR(m_swapChain->ResizeBuffers(1, m_resolution.x, m_resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ComPtr<ID3D11Texture2D> backBuffer;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
	HR(m_device->CreateRenderTargetView(backBuffer.Get(), 0, m_RTV.GetAddressOf()));
	

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};

	depthStencilDesc.Width = static_cast<UINT>(m_resolution.x);
	depthStencilDesc.Height = static_cast<UINT>(m_resolution.y);
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Quality = 1;
	depthStencilDesc.SampleDesc.Count = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_device->CreateTexture2D(&depthStencilDesc, 0, m_DSVBuffer.GetAddressOf()));
	HR(m_device->CreateDepthStencilView(m_DSVBuffer.Get(), nullptr, m_DSV.GetAddressOf()));

	CONTEXT->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());

	m_VP.TopLeftX = 0;
	m_VP.TopLeftY = 0;
	m_VP.Width = m_resolution.x;
	m_VP.Height = m_resolution.y;
	m_VP.MaxDepth = 1;
	m_VP.MinDepth = 0;

	CONTEXT->RSSetViewports(1, &m_VP);
}

HRESULT CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};

	desc.BufferDesc.Width = static_cast<UINT>(m_resolution.x);
	desc.BufferDesc.Height = static_cast<UINT>(m_resolution.y);

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

	desc.Windowed = m_window;
	desc.OutputWindow = m_hWnd;
	desc.Flags = 0;


	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory* dxgiFactory = nullptr;

	m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	HR(dxgiFactory->CreateSwapChain(m_device.Get(), &desc, m_swapChain.GetAddressOf()));

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
	HR(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_RTV.GetAddressOf()));
	

	//DRV
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = static_cast<UINT>(m_resolution.x);
	desc.Height = static_cast<UINT>(m_resolution.y);

	desc.MipLevels = 1;
	desc.ArraySize = 1;

	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	HR(m_device->CreateTexture2D(&desc, 0, m_DSVBuffer.GetAddressOf()));
	
	HR(m_device->CreateDepthStencilView(m_DSVBuffer.Get(), nullptr, m_DSV.GetAddressOf()))

	m_context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());

	return S_OK;
}

HRESULT CDevice::CreateConstBuffer()
{
	for (int i = 0; i < m_constBuffers.size(); ++i)
	{
		m_constBuffers[i] = new CConstBuffer;
	}


	HR(m_constBuffers[(UINT)CB_TYPE::TRANSFORM]->Create(L"Transform", sizeof(Vec4), (UINT)CB_TYPE::TRANSFORM));

	return S_OK;
}

HRESULT CDevice::CreateSampler()
{
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;

	HR(DEVICE->CreateSamplerState(&desc, m_samplers[0].GetAddressOf()));


	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;

	HR(DEVICE->CreateSamplerState(&desc, m_samplers[1].GetAddressOf()));


	ID3D11SamplerState* sam[2] = { m_samplers[0].Get(), m_samplers[1].Get() };

	CONTEXT->VSSetSamplers(0, 2, sam);
	CONTEXT->HSSetSamplers(0, 2, sam);
	CONTEXT->DSSetSamplers(0, 2, sam);
	CONTEXT->GSSetSamplers(0, 2, sam);
	CONTEXT->PSSetSamplers(0, 2, sam);
	CONTEXT->CSSetSamplers(0, 2, sam);

	return S_OK;
}
