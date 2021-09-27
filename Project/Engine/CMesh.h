#pragma once
#include "CResource.h"


/// <summary>
/// ������ �� �ִ� ������������ ������ ����� Ŭ����
/// 
/// �ۼ��� �ֹ���
/// 
/// ���� 
/// 
/// 1.0 - �⺻���� �ۼ� 2021-09-27    
/// </summary>
class CMesh :
    public CResource
{
public:
    CMesh();
    virtual ~CMesh();

public:
    void Create();
public:
    ID3D11Buffer* GetVtxBuffer() { return m_vtxBuffer.Get(); }
    ID3D11Buffer* GetIdxBuffer() { return m_idxBuffer.Get(); }

private:
    ComPtr<ID3D11Buffer> m_vtxBuffer;
    ComPtr<ID3D11Buffer> m_idxBuffer;
    
    VTX*  m_vtxSysmem;
    UINT* m_idxSysmem;
};

