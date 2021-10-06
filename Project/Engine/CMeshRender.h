#pragma once
#include "CComponent.h"


class CTexture;
class CMesh;
class CShader;

class CMeshRender :
    public CComponent
{
public:
    CMeshRender();
    ~CMeshRender();
public:
    virtual void Render();

    virtual void UpdateData(){}

public:
    void SetTexture(CTexture* tex) { texture_ = tex; }
    void SetShader(CShader* shader) { shader_ = shader; }
    void SetMesh(CMesh* mesh) { mesh_ = mesh; }
private:
    CTexture* texture_;
    CMesh* mesh_;
    CShader* shader_;
};

