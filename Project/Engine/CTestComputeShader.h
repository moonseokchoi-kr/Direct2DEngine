#pragma once
#include "CComputeShader.h"
#include "CTexture.h"
#include "ptr.h"

class CTestComputeShader :
    public CComputeShader
{
public:
    CTestComputeShader();
    ~CTestComputeShader();

public:
	// CComputeShader을(를) 통해 상속됨
	virtual void Excute() override;
	virtual void UpdateData() override;
	virtual void Clear() override;
public:
    void SetTargetTexture(Ptr<CTexture>& texture) { target_texture_ = texture; }
    void SetInt(UINT i) { parameter_.int_arr[0] = i; }

    void SetBuffer(CStructuredBuffer* buffer) { test_buffer_ = buffer; }
private:
    Ptr<CTexture> target_texture_;
    CStructuredBuffer* test_buffer_;



};

