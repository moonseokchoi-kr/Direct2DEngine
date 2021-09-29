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
    void SetPipelineState(UINT _pipelineState) { m_pipelineState = _pipelineState; }

private:
    ComPtr<ID3D11Buffer> m_constBuffer;
    D3D11_BUFFER_DESC m_desc;

    UINT m_pipelineState;
    UINT m_registerNum;
};

