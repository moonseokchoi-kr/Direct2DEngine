#pragma once
#include "CResource.h"
class CShader :
    public CResource
{
public:
    CShader();
    ~CShader();

public:
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) { m_topology = _topology; }
    HRESULT CreateVtxShader(const wstring& _strFilePath, const char* _funcName);
    HRESULT CreatePxlShader(const wstring& _strFilePath, const char* _funcName);

    virtual void UpdateData();
private:
	ComPtr<ID3D10Blob> m_vsBlob;
	ComPtr<ID3D10Blob> m_hsBlob;
	ComPtr<ID3D10Blob> m_dsBlob;
	ComPtr<ID3D10Blob> m_gsBlob;
	ComPtr<ID3D10Blob> m_psBlob;
    
    ComPtr<ID3D10Blob> m_errBlob;

    ComPtr<ID3D11VertexShader> m_vtxShader;
    ComPtr<ID3D11HullShader> m_hullShader;
    ComPtr<ID3D11DomainShader> m_domainShader;
    ComPtr<ID3D11GeometryShader> m_geoShader;
    ComPtr<ID3D11PixelShader> m_pxlShader;

    ComPtr<ID3D11InputLayout> m_layout;

    D3D11_PRIMITIVE_TOPOLOGY m_topology;
    
};

