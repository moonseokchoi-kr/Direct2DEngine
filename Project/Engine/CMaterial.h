#pragma once
#include "CResource.h"

class CShader;
class CTexture;

class CMaterial :
    public CResource
{
public:
    CMaterial();
    ~CMaterial();

public:
    void SetData(SHADER_PARAM param, void* data);
	void UpdateData() override;
public:
    void SetShader(CShader* shader) { shader_ = shader; }
    CShader* GetShader() { return shader_; }
private:
    MaterialParameter data_;
    array<CTexture*, static_cast<UINT>(SHADER_PARAM::TEX_END) - static_cast<UINT>(SHADER_PARAM::TEX_0)> texture_;
    CShader* shader_;
};

