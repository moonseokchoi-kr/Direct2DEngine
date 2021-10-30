#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"
CMesh::CMesh()
	:m_vtxSysmem(nullptr)
	,m_vtxCount(0)
	,m_idxSysmem(nullptr)
	,m_idxCount(0)
{
}

CMesh::~CMesh()
{
}

void CMesh::Create(VTX* _vtx, UINT _vtxCount, UINT* _idx, UINT _idxCount)
{
	m_vtxCount = _vtxCount;
	m_idxCount = _idxCount;

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(VTX) * _vtxCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = _vtx;

	if (FAILED(DEVICE->CreateBuffer(&desc, &sub, m_vtxBuffer.GetAddressOf())))
	{
		assert(nullptr);
	}

	//ÀÎµ¦½º¹öÆÛ

	desc.ByteWidth = sizeof(UINT) * _idxCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;

	sub.pSysMem = _idx;

	if (FAILED(DEVICE->CreateBuffer(&desc, &sub, m_idxBuffer.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void CMesh::Render()
{
	UpdateData();
	CONTEXT->DrawIndexed(m_idxCount, 0, 0);
}

void CMesh::UpdateData()
{
	UINT stride = sizeof(VTX);
	UINT offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_vtxBuffer.GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(m_idxBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
