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
    HRESULT Load(const wstring& _strFilePath);

    // CResource을(를) 통해 상속됨
    virtual void UpdateData() override;

public:
    void SetPipelineStage(UINT _pipelineStage, UINT _registerNum)
    {
        m_piplineStage = _pipelineStage;
        m_registerNum = _registerNum;
    }
private:
    ScratchImage m_image;
    ComPtr<ID3D11Texture2D> m_tex2D;
    ComPtr<ID3D11ShaderResourceView> m_SRV;
    D3D11_TEXTURE2D_DESC m_desc;
    
    UINT m_piplineStage;
    UINT m_registerNum;

};

