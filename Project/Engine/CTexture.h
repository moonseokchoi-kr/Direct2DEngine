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

    // CResource��(��) ���� ��ӵ�
    virtual void UpdateData() override;

private:
    ScratchImage m_image;
    ComPtr<ID3D11Texture2D> m_tex2D;
    ComPtr<ID3D11ShaderResourceView> m_SRV;
    D3D11_TEXTURE2D_DESC m_desc;

};
