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
    void RenderParticle(UINT renderCount);
    virtual void UpdateData();
    
public:

private:
    ComPtr<ID3D11Buffer> vertex_buffer_;
    ComPtr<ID3D11Buffer> index_buffer_;
    
    VTX*  vertex_sys_mem_;
    UINT* index_sys_mem_;

    UINT vertex_count_;
    UINT index_count_;
};

