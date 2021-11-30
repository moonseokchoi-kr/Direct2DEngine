#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	:buffer_desc_{}
	, buffer_type_(STRUCTURE_BUFFER_TYPE::READ_ONLY)
	, pipeline_stage_(0)
	, register_number_(0)
	, register_number_rw_(0)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

HRESULT CStructuredBuffer::Create(UINT elementSize, UINT elementCount, STRUCTURE_BUFFER_TYPE type, void* initialData, bool cpuAccess)
{
	//버퍼 초기화
	structured_buffer_ = nullptr;
	shader_resource_view_ = nullptr;
	unordered_access_view_ = nullptr;

	structured_buffer_cpu_read_ = nullptr;
	structured_buffer_cpu_write_ = nullptr;

	buffer_desc_ = D3D11_BUFFER_DESC{};
	buffer_desc_cpu_read_ = D3D11_BUFFER_DESC{};
	buffer_desc_cpu_write_ = D3D11_BUFFER_DESC{};

	//버퍼 생성
	element_size_ = elementSize;
	element_count_ = elementCount;
	buffer_type_ = type;
	cpu_accessable_ = cpuAccess;


	buffer_desc_.ByteWidth = element_size_ * element_count_;

	if (STRUCTURE_BUFFER_TYPE::READ_ONLY == buffer_type_)
	{
		buffer_desc_.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else if (STRUCTURE_BUFFER_TYPE::READ_WRITE == buffer_type_)
	{
		buffer_desc_.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
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

	if (STRUCTURE_BUFFER_TYPE::READ_WRITE == buffer_type_)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.NumElements = element_count_;
		HR(DEVICE->CreateUnorderedAccessView(structured_buffer_.Get(), &uavDesc, unordered_access_view_.GetAddressOf()));
	}

	if (cpu_accessable_)
	{
		buffer_desc_cpu_read_.ByteWidth = element_size_ * element_count_;
		buffer_desc_cpu_read_.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		buffer_desc_cpu_read_.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc_cpu_read_.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		buffer_desc_cpu_read_.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffer_desc_cpu_read_.StructureByteStride = element_size_;

		HR(DEVICE->CreateBuffer(&buffer_desc_cpu_read_, nullptr, structured_buffer_cpu_read_.GetAddressOf()));

		buffer_desc_cpu_write_.ByteWidth = element_size_ * element_count_;
		buffer_desc_cpu_write_.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		buffer_desc_cpu_write_.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc_cpu_write_.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		buffer_desc_cpu_write_.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffer_desc_cpu_write_.StructureByteStride = element_size_;

		HR(DEVICE->CreateBuffer(&buffer_desc_cpu_write_, nullptr, structured_buffer_cpu_write_.GetAddressOf()));

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

void CStructuredBuffer::GetData(void* dest, UINT size)
{
	CONTEXT->CopyResource(structured_buffer_cpu_read_.Get(), structured_buffer_.Get());

	D3D11_MAPPED_SUBRESOURCE sub = {};
	CONTEXT->Map(structured_buffer_cpu_read_.Get(), 0, D3D11_MAP_READ, 0, &sub);
	memcpy(dest, sub.pData, size);
	CONTEXT->Unmap(structured_buffer_cpu_read_.Get(), 0);
}

void CStructuredBuffer::SetData(void* src, UINT size)
{
	D3D11_MAPPED_SUBRESOURCE sub = {};
	CONTEXT->Map(structured_buffer_cpu_write_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, src, size);
	CONTEXT->Unmap(structured_buffer_cpu_write_.Get(), 0);
	CONTEXT->CopyResource(structured_buffer_.Get(), structured_buffer_cpu_write_.Get());
}
