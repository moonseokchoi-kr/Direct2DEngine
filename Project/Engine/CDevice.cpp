#include "pch.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTexture.h"

#include "CResourceManager.h"

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
	Safe_Delete_Array(const_buffer_array_);
}
HRESULT CDevice::Init(HWND mainHWnd, Vec2 resoultion)
{
	hWnd_ = mainHWnd;
	resolution_ = resoultion;

	UINT deviceFlag = 0;
#ifdef _DEBUG
	deviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlag, 0, 0, D3D11_SDK_VERSION, &device_, &featureLevel, &context_)))
	{
		MessageBox(nullptr, L"Direct 초기화 실패", L"Engine 초기화 오류", MB_OK);
		return E_FAIL;
	}
	
	//그래픽카드가 Direct를 지원하는지 확인
	//4x MSAA(multi samples anti-aliasing)지원 확인
	//11부터는 이걸이용해 지원이 되는지 안되는지 확인가능(무조건 4x MSAA를 지원하기때문)
	UINT multiSmapleLevel = 0;
	device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &multiSmapleLevel);
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

	view_port_.TopLeftX = 0;
	view_port_.TopLeftY = 0;

	view_port_.Width = resolution_.x;
	view_port_.Height = resolution_.y;

	view_port_.MinDepth = 0;
	view_port_.MaxDepth = 1;

	context_->RSSetViewports(1, &view_port_);

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

	if (FAILED(CreateBlendState()))
	{
		MessageBox(nullptr, L"BlendState 생성 실패", L"Engine초기화 오류", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateRasterizerState()))
	{
		MessageBox(nullptr, L"Rasterizer 초기화 실패", L"Engine초기화 오류", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateDepthStencilState()))
	{
		MessageBox(nullptr, L"DepthStencil 초기화 실패", L"Engine초기화 오류", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::ClearTarget()
{
	float color[4] = { 0.0f,0.0f,0.0f,0.0f };
	context_->ClearRenderTargetView(render_target_texture_->GetRenderTargetView(), color);
	context_->ClearDepthStencilView(depth_stencil_texture_->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f ,0);
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

	render_target_texture_->GetRenderTargetView()->Release();
	depth_stencil_texture_->GetDepthStencilView()->Release();


	HR(swap_chain_->ResizeBuffers(1, static_cast<UINT>(resolution_.x), static_cast<UINT>(resolution_.y), DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ComPtr<ID3D11Texture2D> backBuffer;
	D3D11_TEXTURE2D_DESC desc;
	HR(swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
	backBuffer->GetDesc(&desc);
	HR(render_target_texture_->Resize(desc.Width,desc.Height));
	HR(depth_stencil_texture_->Resize((UINT)resolution_.x, (UINT)resolution_.y));


	ID3D11RenderTargetView* rtv = render_target_texture_->GetRenderTargetView();
	CONTEXT->OMSetRenderTargets(1, &rtv, depth_stencil_texture_->GetDepthStencilView());

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
	
	render_target_texture_ = CResourceManager::GetInst()->CreateTexture(L"RenderTargetTexture", backBuffer).Get();

	depth_stencil_texture_ = CResourceManager::GetInst()->CreateTexture(L"DepthStencilTexture", (UINT)resolution_.x, (UINT)resolution_.y, D3D11_BIND_DEPTH_STENCIL, DXGI_FORMAT_D24_UNORM_S8_UINT).Get();


	ID3D11RenderTargetView* rtv = render_target_texture_->GetRenderTargetView();
	context_->OMSetRenderTargets(1, &rtv, depth_stencil_texture_->GetDepthStencilView());

	return S_OK;
}

HRESULT CDevice::CreateConstBuffer()
{
	for (int i = 0; i < const_buffer_array_.size(); ++i)
	{
		const_buffer_array_[i] = new CConstBuffer;
	}

	HR(const_buffer_array_[static_cast<UINT>(CB_TYPE::TRANSFORM)]->Create(L"Transform", sizeof(Transform), static_cast<UINT>(CB_TYPE::TRANSFORM)));
	HR(const_buffer_array_[static_cast<UINT>(CB_TYPE::MATERIAL_CONST)]->Create(L"Material", sizeof(MaterialParameter), static_cast<UINT>(CB_TYPE::MATERIAL_CONST)));
	HR(const_buffer_array_[static_cast<UINT>(CB_TYPE::LIGHT2D)]->Create(L"Light2D",sizeof(LightInfo)*50+16, static_cast<UINT>(CB_TYPE::LIGHT2D)))
	HR(const_buffer_array_[static_cast<UINT>(CB_TYPE::ANIMATE2D)]->Create(L"Animate2D",sizeof(AnimationData)*50, static_cast<UINT>(CB_TYPE::ANIMATE2D)))
	return S_OK;
}

HRESULT CDevice::CreateBlendState()
{
	//ALPHA BLEND
	D3D11_BLEND_DESC desc = {};

	//투명한 부분에 대한 깊이 값 기록을 안함
	desc.AlphaToCoverageEnable = true;
	desc.IndependentBlendEnable = false;
	
	desc.RenderTarget[0].BlendEnable = true;

	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(DEVICE->CreateBlendState(&desc, blend_state_array_[static_cast<UINT>(BLEND_TYPE::ALPHA_BLEND)].GetAddressOf()));

	//one one blend

	desc.RenderTarget[0].BlendEnable = true;

	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(DEVICE->CreateBlendState(&desc, blend_state_array_[static_cast<UINT>(BLEND_TYPE::ONE_ONE)].GetAddressOf()));


	return S_OK;
}

HRESULT CDevice::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {};

	desc.CullMode = D3D11_CULL_FRONT;
	desc.FillMode = D3D11_FILL_SOLID;

	HR(DEVICE->CreateRasterizerState(&desc, rasterizer_state_array_[static_cast<UINT>(RASTERIZER_TYPE::CULL_FRONT)].GetAddressOf()));


	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_SOLID;

	HR(DEVICE->CreateRasterizerState(&desc, rasterizer_state_array_[static_cast<UINT>(RASTERIZER_TYPE::CULL_NONE)].GetAddressOf()));


	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_WIREFRAME;

	HR(DEVICE->CreateRasterizerState(&desc, rasterizer_state_array_[static_cast<UINT>(RASTERIZER_TYPE::CULL_WIREFRAME)].GetAddressOf()));
	
	return S_OK;
}

HRESULT CDevice::CreateDepthStencilState()
{
	depth_stencil_state_array_[static_cast<UINT>(DEPTH_STENCIL_TYPE::LESS)] = nullptr;
	
	//LESS EQUAL
	D3D11_DEPTH_STENCIL_DESC desc = {};
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.StencilEnable = false;
	HR(DEVICE->CreateDepthStencilState(&desc, depth_stencil_state_array_[static_cast<UINT>(DEPTH_STENCIL_TYPE::LESS_EQUAL)].GetAddressOf()));
	//GREATER
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_GREATER;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.StencilEnable = false;
	HR(DEVICE->CreateDepthStencilState(&desc, depth_stencil_state_array_[static_cast<UINT>(DEPTH_STENCIL_TYPE::GRATER)].GetAddressOf()));
	//NO_TEST
	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.StencilEnable = false;
	HR(DEVICE->CreateDepthStencilState(&desc, depth_stencil_state_array_[static_cast<UINT>(DEPTH_STENCIL_TYPE::NO_TEST)].GetAddressOf()));
	//NO_WRITE
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable = false;
	HR(DEVICE->CreateDepthStencilState(&desc, depth_stencil_state_array_[static_cast<UINT>(DEPTH_STENCIL_TYPE::NO_WRITE)].GetAddressOf()));
	//NO_TEST_NO_WRITE
	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable = false;
	HR(DEVICE->CreateDepthStencilState(&desc, depth_stencil_state_array_[static_cast<UINT>(DEPTH_STENCIL_TYPE::NO_TEST_NO_WRITE)].GetAddressOf()));
	return S_OK;
}

HRESULT CDevice::CreateSampler()
{
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;

	HR(DEVICE->CreateSamplerState(&desc, sampler_state_array_[0].GetAddressOf()));


	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;

	HR(DEVICE->CreateSamplerState(&desc, sampler_state_array_[1].GetAddressOf()));


	ID3D11SamplerState* sam[2] = { sampler_state_array_[0].Get(), sampler_state_array_[1].Get() };

	CONTEXT->VSSetSamplers(0, 2, sam);
	CONTEXT->HSSetSamplers(0, 2, sam);
	CONTEXT->DSSetSamplers(0, 2, sam);
	CONTEXT->GSSetSamplers(0, 2, sam);
	CONTEXT->PSSetSamplers(0, 2, sam);
	CONTEXT->CSSetSamplers(0, 2, sam);

	return S_OK;
}
