#pragma once
#include "CResource.h"

class CShader :
    public CResource
{
public:
    CShader(RESOURCE_TYPE type);
    ~CShader();
protected:
    
    ComPtr<ID3D10Blob> error_blob_;


    
};

