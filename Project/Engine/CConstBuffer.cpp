#include "pch.h"
#include "CConstBuffer.h"


#include "CDevice.h"
CConstBuffer::CConstBuffer()
	:desc_{}
	,pipeline_stage_(0)
	,register_num_(0)
{
}

CConstBuffer::~CConstBuffer()
{
}

void CConstBuffer::UpdateData()
{
	if (pipeline_stage_ & PS_VERTEX)
	{
		CONTEXT->VSSetConstantBuffers(register_num_, 1, const_buffer_.GetAddressOf());

	}
	if (pipeline_stage_ & PS_HULL)
	{
		CONTEXT->HSSetConstantBuffers(register_num_, 1, const_buffer_.GetAddressOf());

	}
	if (pipeline_stage_ & PS_DOMAIN)
	{
		CONTEXT->DSSetConstantBuffers(register_num_, 1, const_buffer_.GetAddressOf());

	}
	if (pipeline_stage_ & PS_GEOMETRY)
	{
		CONTEXT->GSSetConstantBuffers(register_num_, 1, const_buffer_.GetAddressOf());

	}
	if (pipeline_stage_ & PS_PIXEL)
	{
		CONTEXT->PSSetConstantBuffers(register_num_, 1, const_buffer_.GetAddressOf());

	}
	if (pipeline_stage_ & PS_COMPUTE)
	{
		CONTEXT->CSSetConstantBuffers(register_num_, 1, const_buffer_.GetAddressOf());
	}

}

HRESULT CConstBuffer::Create(const wstring& _strName, UINT _bufferSize, UINT _registerNumber)
{
	SetName(_strName);

	register_num_ = _registerNumber;

	desc_.ByteWidth = _bufferSize;
	desc_.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc_.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc_.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(DEVICE->CreateBuffer(&desc_, nullptr, const_buffer_.GetAddressOf())))
	{
		assert(nullptr);
		return E_FAIL;
	}

	return S_OK;
}

void CConstBuffer::SetData(void* _data, UINT _size)
{
	assert(!(desc_.ByteWidth < _size));

	D3D11_MAPPED_SUBRESOURCE subRes = {};

	CONTEXT->Map(const_buffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);

	memcpy(subRes.pData, _data, _size);

	CONTEXT->Unmap(const_buffer_.Get(), 0);
}
