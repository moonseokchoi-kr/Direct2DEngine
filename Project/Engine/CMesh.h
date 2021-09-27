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

