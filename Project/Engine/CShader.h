#pragma once
#include "CResource.h"
class CShader :
    public CResource
{
public:
    CShader();
    ~CShader();
protected:
    
    ComPtr<ID3D10Blob> m_errBlob;


    
};

