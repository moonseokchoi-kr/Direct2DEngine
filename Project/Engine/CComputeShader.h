#pragma once
#include "CShader.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CResourceManager.h"
#include "CTimeManager.h"
#include "CMaterial.h"
class CComputeShader :
    public CShader
{
public:
    CComputeShader();
    CComputeShader(UINT groupX, UINT groupY, UINT groupZ);
    ~CComputeShader();

public:
    HRESULT CreateComputeShader(const wstring& _strFilePath, const char* _funcName);
    virtual void  Excute() = 0;

 
protected:
    virtual void UpdateData() = 0;
    virtual void Clear() = 0;
    int GetGroupPerThreadX() { return group_thread_x_; }
    int GetGroupPerThreadY() { return group_thread_y_; }
    int GetGroupPerThreadZ() { return group_thread_z_; }

protected:
    void Dispatch(UINT groupX, UINT groupY, UINT groupZ);
protected:
    MaterialParameter parameter_;

private:
    ComPtr<ID3DBlob> compute_shader_blob_;
    ComPtr<ID3D11ComputeShader> compute_shader_;
    
    int group_thread_x_;
    int group_thread_y_;
    int group_thread_z_;

};

