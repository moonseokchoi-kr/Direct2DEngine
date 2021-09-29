#include "pch.h"
#include "CConstBuffer.h"


#include "CDevice.h"
CConstBuffer::CConstBuffer()
	:m_desc{}
	,m_pipelineState(0)
	,m_registerNum(0)
{
}

CConstBuffer::~CConstBuffer()
{
}

void CConstBuffer::UpdateData()
{
	if (m_pipelineState & PS_VERTEX)
	{
		CONTEXT->VSSetConstantBuffers(m_registerNum, 1, m_constBuffer.GetAddressOf());

	}
	if (m_pipelineState & PS_HULL)
	{
		CONTEXT->HSSetConstantBuffers(m_registerNum, 1, m_constBuffer.GetAddressOf());

	}
	if (m_pipelineState & PS_DOMAIN)
	{
		CONTEXT->DSSetConstantBuffers(m_registerNum, 1, m_constBuffer.GetAddressOf());

	}
	if (m_pipelineState & PS_GEOMETRY)
	{
		CONTEXT->GSSetConstantBuffers(m_registerNum, 1, m_constBuffer.GetAddressOf());

	}
	if (m_pipelineState & PS_PIXEL)
	{
		CONTEXT->PSSetConstantBuffers(m_registerNum, 1, m_constBuffer.GetAddressOf());

	}

}

HRESULT CConstBuffer::Create(const wstring& _strName, UINT _bufferSize, UINT _registerNumber)
{
	SetName(_strName);

	m_registerNum = _registerNumber;

	m_desc.ByteWidth = _bufferSize;
	m_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_desc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(DEVICE->CreateBuffer(&m_desc, nullptr, m_constBuffer.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CConstBuffer::SetData(void* _data, UINT _size)
{
	D3D11_MAPPED_SUBRESOURCE subRes = {};

	CONTEXT->Map(m_constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);

	memcpy(subRes.pData, _data, _size);

	CONTEXT->Unmap(m_constBuffer.Get(), 0);
}
