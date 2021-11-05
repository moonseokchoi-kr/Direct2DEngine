#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	:buffer_desc_{}
	,buffer_type_(STRUCTURE_BUFFER_TYPE::READ_ONLY)
	,pipeline_stage_(-1)
	,register_number_(-1)
	,register_number_rw_(-1)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

HRESULT CStructuredBuffer::Create(UINT elementSize, UINT elementCount, STRUCTURE_BUFFER_TYPE type, void* initialData)
{
	element_size_ = elementSize;
	element_count_ = elementCount;
	buffer_type_ = type;

	buffer_desc_.ByteWidth = element_size_ * element_count_;

	if (STRUCTURE_BUFFER_TYPE::READ_ONLY == buffer_type_)
	{
		buffer_desc_.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else if (STRUCTURE_BUFFER_TYPE::READ_WRITE == buffer_type_)
	{
		buffer_desc_.BindFlags = D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_UNORDERED_ACCESS;
	}
	else
	{

	}

	buffer_desc_.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc_.CPUAccessFlags = 0;

	buffer_desc_.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	buffer_desc_.StructureByteStride = element_size_;

	if (nullptr != initialData)
	{
		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = initialData;
		HR(DEVICE->CreateBuffer(&buffer_desc_, &data, structured_buffer_.GetAddressOf()));
	}
	else
	{
		HR(DEVICE->CreateBuffer(&buffer_desc_, nullptr, structured_buffer_.GetAddressOf()));
	}

	//ShaderResourceView
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.BufferEx.NumElements = element_count_;

	HR(DEVICE->CreateShaderResourceView(structured_buffer_.Get(), &srvDesc, shader_resource_view_.GetAddressOf()));

	if (STRUCTURE_BUFFER_TYPE::CPU_ACCESS == buffer_type_||STRUCTURE_BUFFER_TYPE::READ_WRITE == buffer_type_)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.NumElements = element_count_;
		HR(DEVICE->CreateUnorderedAccessView(structured_buffer_.Get(), &uavDesc, unordered_access_view_.GetAddressOf()));
	}

	return S_OK;
}

void CStructuredBuffer::UpdateData(UINT piplineStage, UINT registerNumber)
{
	pipeline_stage_ = piplineStage;
	register_number_ = registerNumber;

	if (pipeline_stage_ & PS_VERTEX)
	{
		CONTEXT->VSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());

	}
	if (pipeline_stage_ & PS_HULL)
	{
		CONTEXT->HSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());

	}
	if (pipeline_stage_ & PS_DOMAIN)
	{
		CONTEXT->DSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());

	}
	if (pipeline_stage_ & PS_GEOMETRY)
	{
		CONTEXT->GSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());

	}
	if (pipeline_stage_ & PS_PIXEL)
	{
		CONTEXT->PSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());

	}
}

void CStructuredBuffer::UpdataDataCS(UINT registerNumber)
{
	register_number_ = registerNumber;
	CONTEXT->CSSetShaderResources(register_number_, 1, shader_resource_view_.GetAddressOf());
}

void CStructuredBuffer::UpdateDataRW_CS(UINT registerNumber)
{
	if (STRUCTURE_BUFFER_TYPE::READ_ONLY == buffer_type_)
	{
		MessageBox(nullptr, L"현재 버퍼는 읽기전용입니다.", L"접근오류", MB_OK);
		assert(nullptr);
	}
	register_number_rw_ = registerNumber;
	ID3D11UnorderedAccessView* uav = unordered_access_view_.Get();
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(register_number_rw_, 1, &uav, &i);
}

void CStructuredBuffer::Clear()
{
	ID3D11ShaderResourceView* sav = nullptr;
	
	CONTEXT->VSSetShaderResources(register_number_, 1, &sav);

	CONTEXT->HSSetShaderResources(register_number_, 1, &sav);

	CONTEXT->DSSetShaderResources(register_number_, 1, &sav);

	CONTEXT->GSSetShaderResources(register_number_, 1, &sav);

	CONTEXT->PSSetShaderResources(register_number_, 1, &sav);
	CONTEXT->CSSetShaderResources(register_number_, 1, &sav);

	ID3D11UnorderedAccessView* uav = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(register_number_rw_, 1, &uav, &i);

}
