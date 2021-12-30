#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"
CMesh::CMesh()
	:CResource(RESOURCE_TYPE::MESH)
	,vertex_sys_mem_(nullptr)
	,vertex_count_(0)
	,index_sys_mem_(nullptr)
	,index_count_(0)
{
}

CMesh::~CMesh()
{
	SafeDelete(vertex_sys_mem_);
	SafeDelete(index_sys_mem_);
}

void CMesh::Create(VTX* _vtx, UINT _vtxCount, UINT* _idx, UINT _idxCount)
{
	vertex_count_ = _vtxCount;
	index_count_ = _idxCount;

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(VTX) * _vtxCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = _vtx;

	if (FAILED(DEVICE->CreateBuffer(&desc, &sub, vertex_buffer_.GetAddressOf())))
	{
		assert(nullptr);
	}

	//ÀÎµ¦½º¹öÆÛ

	desc.ByteWidth = sizeof(UINT) * _idxCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;

	sub.pSysMem = _idx;

	if (FAILED(DEVICE->CreateBuffer(&desc, &sub, index_buffer_.GetAddressOf())))
	{
		assert(nullptr);
	}

	vertex_sys_mem_ = new Vertex[vertex_count_];
	memcpy(vertex_sys_mem_, _vtx, sizeof(Vertex) * vertex_count_);
	index_sys_mem_ = new UINT[index_count_];
	memcpy(index_sys_mem_, _idx, sizeof(UINT) * index_count_);
	
}

void CMesh::Create(VTX* _vtx, UINT _vtxCount)
{
	vertex_count_ = _vtxCount;

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(VTX) * _vtxCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = _vtx;

	if (FAILED(DEVICE->CreateBuffer(&desc, &sub, vertex_buffer_.GetAddressOf())))
	{
		assert(nullptr);
	}

}

void CMesh::Render()
{
	UpdateData();
	CONTEXT->DrawIndexed(index_count_, 0, 0);
}

void CMesh::RenderNoneIndexed()
{
	UpdateData();
	CONTEXT->Draw(vertex_count_, 0);
}

void CMesh::RenderParticle(UINT renderCount)
{
	UpdateData();
	CONTEXT->DrawIndexedInstanced(index_count_, renderCount, 0, 0, 0);
}

void CMesh::UpdateData()
{
	UINT stride = sizeof(VTX);
	UINT offset = 0;
	if(nullptr != vertex_buffer_)
		CONTEXT->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
	if(nullptr != index_buffer_)
		CONTEXT->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
}
