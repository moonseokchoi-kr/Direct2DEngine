#pragma once
#include "CResource.h"
#include "CTexture.h"
#include "CGraphicsShader.h"

class CMaterial :
    public CResource
{

public:
    CMaterial();
    CMaterial(bool _defualt);
    CMaterial(const CMaterial& origin);
    ~CMaterial();

public:
    void SetData(SHADER_PARAM param, void* data);
    void GetData(SHADER_PARAM param, void* data);
	void UpdateData() override;

    CLONE(CMaterial);
public:
    void SetShader(Ptr<CGraphicsShader> shader) { shader_ = shader; }
    Ptr<CGraphicsShader> GetShader() { return shader_; }

public:
    HRESULT Save(const wstring& relativePath) override;
    HRESULT Load(const wstring& filePath) override;
private:
	MaterialParameter data_;
	array<Ptr<CTexture>, static_cast<UINT>(SHADER_PARAM::TEX_END) - static_cast<UINT>(SHADER_PARAM::TEX_0)> texture_array_;
	Ptr<CGraphicsShader> shader_;
    bool is_default;

};

