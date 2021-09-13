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
	D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;
	
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlag, 0, 0, D3D11_SDK_VERSION, &m_device, &level, &m_context)))
	{
		MessageBox(nullptr, L"Direct �ʱ�ȭ ����", L"Engine �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}
	
	//�׷���ī�尡 Direct�� �����ϴ��� Ȯ��
	UINT multiSmapleLevel = 0;
	m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &multiSmapleLevel);
	if (0 == multiSmapleLevel)
	{
		MessageBox(nullptr, L"�ϵ��� Direct11�� �������� ����", L"Engine�ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"����ü�� ���� ����", L"Engine�ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateView()))
	{
		MessageBox(nullptr, L"�� ��������", L"Engine�ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};

	desc.BufferDesc.Width = m_resolution.x;
	desc.BufferDesc.Height = m_resolution.y;

	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//���Ŀ� ���������� ���氡���ϵ��� �ϱ�
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

	if (FAILED(dxgiFactory->CreateSwapChain(m_device, &desc, &m_swapChain)))
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
	return S_OK;
}

HRESULT CDevice::CreateViewport()
{
	return S_OK;
}
