#pragma once
#include "CResource.h"
#include "CGraphicsShader.h"

class CTexture;
class CGraphicsShader;

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
    void SetShader(Ptr<CGraphicsShader> shader) { shader_ = shader; }
    Ptr<CGraphicsShader> GetShader() { return shader_; }

private:
	MaterialParameter data_;
	array<Ptr<CTexture>, static_cast<UINT>(SHADER_PARAM::TEX_END) - static_cast<UINT>(SHADER_PARAM::TEX_0)> texture_array_;
	Ptr<CGraphicsShader> shader_;

};

