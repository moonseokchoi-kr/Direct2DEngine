#include "pch.h"

#include "CDevice.h"
#include "CConstBuffer.h"
CDevice::CDevice()
	:hWnd_(nullptr)
	,device_(nullptr)
	,context_(nullptr)
	,window_(true)
	, view_port_{}
{

}
CDevice::~CDevice()
{
}
HRESULT CDevice::Init(HWND mainHWnd, Vec2 resoultion)
{
	hWnd_ = mainHWnd;
	resolution_ = resoultion;

	adapter_ = GrapicsAdapterUtil::EnumratesAdapters(L"NVIDIA GeForce GTX 1660 Ti");
	//GrapicsAdapterUtil::EnumratesOutputs(adapter_);
	UINT deviceFlag = 0;
#ifdef _DEBUG
	deviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlag, 0, 0, D3D11_SDK_VERSION, &device_, &featureLevel, &context_)))
	{
		MessageBox(nullptr, L"Direct �ʱ�ȭ ����", L"Engine �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}
	
	//�׷���ī�尡 Direct�� �����ϴ��� Ȯ��
	//4x MSAA(multi samples anti-aliasing)���� Ȯ��
	//11���ʹ� �̰��̿��� ������ �Ǵ��� �ȵǴ��� Ȯ�ΰ���(������ 4x MSAA�� �����ϱ⶧��)
	UINT multiSmapleLevel = 0;
	device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &multiSmapleLevel);
	if (0 == multiSmapleLevel)
	{
		MessageBox(nullptr, L"�ϵ��� 4xMSAA�� �������� �ʽ��ϴ�", L"Engine�ʱ�ȭ ����", MB_OK);
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

	view_port_.TopLeftX = 0;
	view_port_.TopLeftY = 0;

	view_port_.Width = resolution_.x;
	view_port_.Height = resolution_.y;

	view_port_.MinDepth = 0;
	view_port_.MaxDepth = 1;

	context_->RSSetViewports(1, &view_port_);

	if (FAILED(CreateSampler()))
	{
		MessageBox(nullptr, L"Sampler ���� ����", L"Engine�ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateConstBuffer()))
	{
		MessageBox(nullptr, L"������� �ʱ�ȭ ����", L"Engine�ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::ClearTarget()
{
	float color[4] = { 0.4f,0.4f,0.4f,1.0f };
	context_->ClearRenderTargetView(render_target_view_.Get(), color);
	context_->ClearDepthStencilView(depth_stencil_view_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f ,0);
}

void CDevice::Present()
{
	swap_chain_.Get()->Present(0, 0);
}

HRESULT CDevice::OnReSize(Vec2 _resolution)
{
	resolution_ = _resolution;

	assert(context_);
	assert(device_);
	assert(swap_chain_);
	//comptr�� ����Ǹ� ����Ƚ���� 0�� �Ǿ�� �������, �ٵ� ���� ������ �������Ű�� �����ϰ� �ִ��ֵ��� �ض߰Ե�, ������ �����ϰ� �ִ� �ֵ��� ������ ��
	//������ �μ��� �Ǵ��� �𸣰���
	render_target_view_->Release();
	render_target_view_ = nullptr;
	depth_stencil_view_->Release();
	depth_stencil_view_ = nullptr;
	depth_stencil_buffer_->Release();
	depth_stencil_buffer_ = nullptr;
	

	HR(swap_chain_->ResizeBuffers(1, static_cast<UINT>(resolution_.x), static_cast<UINT>(resolution_.y), DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ComPtr<ID3D11Texture2D> backBuffer;
	HR(swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
	HR(device_->CreateRenderTargetView(backBuffer.Get(), 0, render_target_view_.GetAddressOf()));
	

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};

	depthStencilDesc.Width = static_cast<UINT>(resolution_.x);
	depthStencilDesc.Height = static_cast<UINT>(resolution_.y);
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Quality = 1;
	depthStencilDesc.SampleDesc.Count = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(device_->CreateTexture2D(&depthStencilDesc, 0, depth_stencil_buffer_.GetAddressOf()));
	HR(device_->CreateDepthStencilView(depth_stencil_buffer_.Get(), nullptr, depth_stencil_view_.GetAddressOf()));

	CONTEXT->OMSetRenderTargets(1, render_target_view_.GetAddressOf(), depth_stencil_view_.Get());

	view_port_.TopLeftX = 0;
	view_port_.TopLeftY = 0;
	view_port_.Width = resolution_.x;
	view_port_.Height = resolution_.y;
	view_port_.MaxDepth = 1;
	view_port_.MinDepth = 0;

	CONTEXT->RSSetViewports(1, &view_port_);

	return S_OK;
}

HRESULT CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};

	desc.BufferDesc.Width = static_cast<UINT>(resolution_.x);
	desc.BufferDesc.Height = static_cast<UINT>(resolution_.y);

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

	desc.Windowed = window_;
	desc.OutputWindow = hWnd_;
	desc.Flags = 0;


	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory* dxgiFactory = nullptr;

	device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(device_.Get(), &desc, swap_chain_.GetAddressOf()));

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	return S_OK;
}

HRESULT CDevice::CreateView()
{
	//RTV
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	HR(device_->CreateRenderTargetView(backBuffer.Get(), nullptr, render_target_view_.GetAddressOf()));
	

	//DRV
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = static_cast<UINT>(resolution_.x);
	desc.Height = static_cast<UINT>(resolution_.y);

	desc.MipLevels = 1;
	desc.ArraySize = 1;

	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	HR(device_->CreateTexture2D(&desc, 0, depth_stencil_buffer_.GetAddressOf()));
	
	HR(device_->CreateDepthStencilView(depth_stencil_buffer_.Get(), nullptr, depth_stencil_view_.GetAddressOf()))

	context_->OMSetRenderTargets(1, render_target_view_.GetAddressOf(), depth_stencil_view_.Get());

	return S_OK;
}

HRESULT CDevice::CreateConstBuffer()
{
	for (int i = 0; i < const_buffers_.size(); ++i)
	{
		const_buffers_[i] = new CConstBuffer;
	}


	HR(const_buffers_[static_cast<UINT>(CB_TYPE::TRANSFORM)]->Create(L"Transform", sizeof(Transform), static_cast<UINT>(CB_TYPE::TRANSFORM)));

	return S_OK;
}

HRESULT CDevice::CreateSampler()
{
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;

	HR(DEVICE->CreateSamplerState(&desc, samplers_[0].GetAddressOf()));


	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;

	HR(DEVICE->CreateSamplerState(&desc, samplers_[1].GetAddressOf()));


	ID3D11SamplerState* sam[2] = { samplers_[0].Get(), samplers_[1].Get() };

	CONTEXT->VSSetSamplers(0, 2, sam);
	CONTEXT->HSSetSamplers(0, 2, sam);
	CONTEXT->DSSetSamplers(0, 2, sam);
	CONTEXT->GSSetSamplers(0, 2, sam);
	CONTEXT->PSSetSamplers(0, 2, sam);
	CONTEXT->CSSetSamplers(0, 2, sam);

	return S_OK;
}
