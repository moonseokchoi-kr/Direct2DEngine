#pragma once
#include "CComponent.h"


class CMaterial;
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
    void Render() override;
    void FinalUpdate() override;
    void UpdateData() override {};
public:
    CMaterial* GetMaterial() { return material_; }
    CMesh* GetMesh() { return mesh_; }
	
public:
    void SetMaterial(CMaterial* material) { material_ = material; }
    void SetMesh(CMesh* mesh) { mesh_ = mesh; }
private:
    CMaterial* material_;
    CMesh* mesh_;
};

