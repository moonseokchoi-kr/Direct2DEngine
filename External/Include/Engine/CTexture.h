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
    // CResource을(를) 통해 상속됨
    virtual void UpdateData() override;
    void UpdateDataRW(UINT registerNumber);
public:
    void SetPipelineStage(UINT pipelineStage, UINT registerNum)
    {
        pipline_stage_ = pipelineStage;
        register_number_ = registerNum;
    }
    UINT GetWidth() { return texture_desc_.Width; }
    UINT GetHeight() { return texture_desc_.Height; }
    ID3D11Texture2D* GetTex2D() { return texture_2D_.Get(); }

    ID3D11RenderTargetView* GetRenderTargetView() { return render_target_texture_.Get(); }
    ID3D11ShaderResourceView* GetShaderResourceView() { return shader_resource_view_.Get(); }
    ID3D11DepthStencilView* GetDepthStencilView() { return depth_stencil_texture_.Get(); }
    ID3D11UnorderedAccessView* GetUnorderAccessView() { return unordered_access_view_.Get(); }
public:
    HRESULT Resize(UINT width, UINT height);
    static void Clear(UINT registerNumber);
    static void ClearRW(UINT registerNumber);
private:

    HRESULT Create(UINT width, UINT height, UINT flags, DXGI_FORMAT format);
    HRESULT Create(ComPtr<ID3D11Texture2D> texture);
	HRESULT Load(const wstring& strFilePath) override;
private:
    ScratchImage image_;
    
    ComPtr<ID3D11Texture2D> texture_2D_;
    
    ComPtr<ID3D11ShaderResourceView> shader_resource_view_;
    ComPtr<ID3D11DepthStencilView> depth_stencil_texture_;
    ComPtr<ID3D11RenderTargetView> render_target_texture_;
    ComPtr<ID3D11UnorderedAccessView> unordered_access_view_;

    D3D11_TEXTURE2D_DESC texture_desc_;
    
    UINT pipline_stage_;
    UINT register_number_;

    friend class CResourceManager;
};

