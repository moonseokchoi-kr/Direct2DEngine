#include "pch.h"
#include "temp.h"
#include "CDevice.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CPathManager.h"

/// <summary>
/// 붉은 사각형 그리기
/// </summary>


ComPtr<ID3D11Buffer> g_VB = nullptr;	//버텍스 버퍼

ComPtr<ID3D11Buffer> g_IB = nullptr;	//인덱스 버퍼	

ComPtr<ID3D11Buffer> g_CB = nullptr;	//상수 버퍼

ComPtr<ID3D11InputLayout> g_inputLayout = nullptr;

ComPtr<ID3D10Blob> g_vsBlob = nullptr; //버텍스셰이더 버퍼
ComPtr<ID3D10Blob> g_psBlob = nullptr; //픽셀셰이더 버퍼
ComPtr<ID3D10Blob> g_errBlob = nullptr; //에러데이터 버퍼

ComPtr<ID3D11VertexShader> g_vs = nullptr;
ComPtr<ID3D11PixelShader> g_ps = nullptr;

Vec4 g_moveDir = Vec4();
void init()
{
	array<VTX, 4>vertexArr = {};

	//투영좌표계기준
	//원래는 좌표전환을 통해 좌표계를 바꾸어야함
	//월드->뷰->프로젝션->투영 순임
	vertexArr[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vertexArr[0].color = Vec4(1.f, 0.f, 0.f, 1.f);

	vertexArr[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vertexArr[1].color = Vec4(0.f, 1.f, 0.f, 1.f);

	vertexArr[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vertexArr[2].color = Vec4(0.f, 0.f, 1.f, 1.f);

	vertexArr[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vertexArr[3].color = Vec4(1.f, 0.f, 1.f, 1.f);

	//버텍스버퍼 설정
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(VTX) * (UINT)vertexArr.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = &vertexArr;

	if (FAILED(DEVICE->CreateBuffer(&desc, &sub, g_VB.GetAddressOf())))
	{
		assert(nullptr);
	}

	//인덱스버퍼
	array<UINT, 6> indexArr = { 0,1,2,0,2,3 };
	desc.ByteWidth = sizeof(UINT) * indexArr.size();
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;

	sub.pSysMem = &indexArr;

	if (FAILED(DEVICE->CreateBuffer(&desc, &sub, g_IB.GetAddressOf())))
	{
		assert(nullptr);
	}

	desc.ByteWidth = sizeof(Vec4);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&desc, &sub, g_CB.GetAddressOf())))
	{
		assert(nullptr);
	}


	//Shader processing

	//vertex

	wstring strPath = CPathManager::GetInst()->GetContentPath();
	strPath += L"shader\\std2d.fx";

	HRESULT hr = D3DCompileFromFile(
		strPath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS_std",
		"vs_5_0",
		D3DCOMPILE_DEBUG,
		0,
		g_vsBlob.GetAddressOf(),
		g_errBlob.GetAddressOf()
	);

	if (FAILED(hr))
	{
		assert(nullptr);
	}

	hr = D3DCompileFromFile(
		strPath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS_std",
		"ps_5_0",
		D3DCOMPILE_DEBUG,
		0,
		g_psBlob.GetAddressOf(),
		g_errBlob.GetAddressOf()
	);

	if (FAILED(hr))
	{
		assert(nullptr);
	}

	DEVICE->CreateVertexShader(g_vsBlob->GetBufferPointer(), g_vsBlob->GetBufferSize(), nullptr, g_vs.GetAddressOf());
	DEVICE->CreatePixelShader(g_psBlob->GetBufferPointer(), g_psBlob->GetBufferSize(), nullptr, g_ps.GetAddressOf());

	//입력 속성정의
	D3D11_INPUT_ELEMENT_DESC tLayout[2] = {};

	//점
	tLayout[0].SemanticName = "POSITION";
	tLayout[0].SemanticIndex = 0;
	tLayout[0].AlignedByteOffset = 0;
	tLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tLayout[0].InputSlot = 0;
	tLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayout[0].InstanceDataStepRate = 0;
	//색
	tLayout[1].SemanticName = "COLOR";
	tLayout[1].SemanticIndex = 0;
	tLayout[1].AlignedByteOffset = sizeof(float) * 3;
	tLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tLayout[1].InputSlot = 0;
	tLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayout[1].InstanceDataStepRate = 0;

	if (FAILED(DEVICE->CreateInputLayout(tLayout, 2, g_vsBlob->GetBufferPointer(), g_vsBlob->GetBufferSize(), g_inputLayout.GetAddressOf())))
	{
		assert(nullptr);
	}
}


void Update()
{
	if (KEY_HOLD(KEY::UP))
	{
		g_moveDir.y += fDT * 0.5f;
	}
	if (KEY_HOLD(KEY::DOWN))
	{
		g_moveDir.y -= fDT * 0.5f;
	}
	if (KEY_HOLD(KEY::LEFT))
	{
		g_moveDir.x -= fDT * 0.5f;
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		g_moveDir.x += fDT * 0.5f;
	}


	D3D11_MAPPED_SUBRESOURCE subRes = {};

	CONTEXT->Map(g_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);

	memcpy(subRes.pData, &g_moveDir, sizeof(Vec4));

	CONTEXT->Unmap(g_CB.Get(), 0);
}

void Render()
{
	CDevice::GetInst()->ClearTarget();

	UINT stride = sizeof(VTX);
	UINT offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(g_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CONTEXT->IASetInputLayout(g_inputLayout.Get());

	CONTEXT->VSSetConstantBuffers(0, 1, g_CB.GetAddressOf());

	CONTEXT->VSSetShader(g_vs.Get(), nullptr, 0);
	CONTEXT->PSSetShader(g_ps.Get(), nullptr, 0);

	CONTEXT->DrawIndexed(6, 0, 0);

	CDevice::GetInst()->Present();
}
