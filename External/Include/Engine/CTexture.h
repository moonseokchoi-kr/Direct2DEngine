#pragma once
#include "CResource.h"

#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#endif
class CTexture :
    public CResource
{
public:
    CTexture();
    ~CTexture();

public:
    HRESULT Load(const wstring& strFilePath);

    // CResource을(를) 통해 상속됨
    virtual void UpdateData() override;

public:
    void SetPipelineStage(UINT pipelineStage, UINT registerNum)
    {
        pipline_stage_ = pipelineStage;
        register_number_ = registerNum;
    }
private:
    ScratchImage image_;
    ComPtr<ID3D11Texture2D> texure_2D_;
    ComPtr<ID3D11ShaderResourceView> shader_resource_view_;
    D3D11_TEXTURE2D_DESC texture_desc_;
    
    UINT pipline_stage_;
    UINT register_number_;

};

