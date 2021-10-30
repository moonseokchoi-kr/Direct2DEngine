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
    void Create(VTX* _vtx, UINT _vtxCount, UINT* _idx, UINT _idxCount);
    void Render();

    virtual void UpdateData();
    
public:

private:
    ComPtr<ID3D11Buffer> m_vtxBuffer;
    ComPtr<ID3D11Buffer> m_idxBuffer;
    
    VTX*  m_vtxSysmem;
    UINT* m_idxSysmem;

    UINT m_vtxCount;
    UINT m_idxCount;
};

