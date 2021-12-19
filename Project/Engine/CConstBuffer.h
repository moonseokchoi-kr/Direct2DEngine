#pragma once
#include "CEntity.h"
class CConstBuffer :
    public CEntity
{
public:
    CConstBuffer();
    ~CConstBuffer();

public:
    virtual void UpdateData();
    HRESULT Create(const wstring& _strName, UINT _bufferSize, UINT _registerNumber);
    void SetData(void* _data, UINT _size);
    void SetPipelineStage(UINT _pipelineStage) { pipeline_stage_ = _pipelineStage; }

public:
    CConstBuffer* Clone() { return nullptr; }

    void Clear();
private:
    ComPtr<ID3D11Buffer> const_buffer_;
    D3D11_BUFFER_DESC desc_;

    UINT pipeline_stage_;
    UINT register_num_;
};

