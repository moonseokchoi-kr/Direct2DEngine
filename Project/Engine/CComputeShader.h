#pragma once
#include "CShader.h"
class CComputeShader :
    public CShader
{
public:
    CComputeShader();
    ~CComputeShader();

public:
    HRESULT CreateComputeShader(const wstring& _strFilePath, const char* _funcName);

    virtual void UpdateData();

private:
    ComPtr<ID3DBlob> compute_shader_blob_;
    ComPtr<ID3D11ComputeShader> compute_shader_;

};

