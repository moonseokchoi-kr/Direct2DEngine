#pragma once
#include "CEntity.h"

enum class STRUCTURE_BUFFER_TYPE
{
    READ_ONLY,
    READ_WRITE,
    CPU_ACCESS,
};

class CStructuredBuffer :
    public CEntity
{
public:
    CStructuredBuffer();
    ~CStructuredBuffer();
public:
    HRESULT Create(UINT elementSize, UINT elementCount, STRUCTURE_BUFFER_TYPE type, void* initialData , bool cpuAccess);

    void UpdateData(UINT piplineStage, UINT registerNumber);
    void UpdataDataCS(UINT registerNumber);
    void UpdateDataRW_CS(UINT registerNumber);

    void Clear();
    virtual void UpdateData() override {};
public:
    UINT GetElementCount() { return element_count_; }
    UINT GetElementSize() { return element_size_; }

    void GetData(void* dest, UINT size);
    void SetData(void* src, UINT size);
public:
    CLONE(CStructuredBuffer);

private:
    ComPtr<ID3D11Buffer>    structured_buffer_;
    D3D11_BUFFER_DESC       buffer_desc_;
    ComPtr<ID3D11ShaderResourceView>    shader_resource_view_;
    ComPtr<ID3D11UnorderedAccessView>   unordered_access_view_;

    ComPtr<ID3D11Buffer> structured_buffer_cpu_read_;
    ComPtr<ID3D11Buffer> structured_buffer_cpu_write_;

    D3D11_BUFFER_DESC       buffer_desc_cpu_read_;
    D3D11_BUFFER_DESC       buffer_desc_cpu_write_;

    UINT    element_size_;
    UINT    element_count_;
    STRUCTURE_BUFFER_TYPE buffer_type_;
    
    UINT    pipeline_stage_;
    UINT    register_number_;
    UINT    register_number_rw_;

    bool    cpu_accessable_;
};

