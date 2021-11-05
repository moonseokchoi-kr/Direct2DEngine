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
    HRESULT Create(UINT elementSize, UINT elementCount, STRUCTURE_BUFFER_TYPE type, void* initialData = nullptr);
    virtual void UpdateData() override;

public:
    CLONE;

};

