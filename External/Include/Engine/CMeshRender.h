#pragma once
#include "CComponent.h"
#include "ptr.h"

class CMaterial;
class CTexture;
class CMesh;

class CMeshRender :
    public CComponent
{
public:
    CMeshRender();
    ~CMeshRender();
public:
    void Render() override;
    void FinalUpdate() override;
    void UpdateData() override {}
public:
    Ptr<CMaterial> GetMaterial() { return material_; }
    Ptr<CMesh> GetMesh() { return mesh_; }
	
public:
    void SetMaterial(Ptr<CMaterial> material) { material_ = material; }
    void SetMesh(Ptr<CMesh> mesh) { mesh_ = mesh; }
private:
    Ptr<CMaterial> material_;
    Ptr<CMesh> mesh_;
};

