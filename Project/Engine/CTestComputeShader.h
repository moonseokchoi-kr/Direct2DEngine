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
	// CComputeShader��(��) ���� ��ӵ�
	virtual void Excute() override;
	virtual void UpdateData() override;
	virtual void Clear() override;
public:
    void SetTexture(Ptr<CTexture>& texture) { target_texture_ = texture; }


private:
    Ptr<CTexture> target_texture_;


};

