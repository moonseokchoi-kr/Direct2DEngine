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
    void SetTexture(CTexture* _tex) { m_texture = _tex; }
    void SetShader(CShader* _shd) { m_shader = _shd; }
    void SetMesh(CMesh* _mesh) { m_mesh = _mesh; }
private:
    CTexture* m_texture;
    CMesh* m_mesh;
    CShader* m_shader;
};

