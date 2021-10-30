#pragma once
#include "CResource.h"


/// <summary>
/// 공유할 수 있는 정점데이터의 원형을 만드는 클래스
/// 
/// 작성자 최문석
/// 
/// 버전 
/// 
/// 1.0 - 기본구조 작성 2021-09-27    
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

