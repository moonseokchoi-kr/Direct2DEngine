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

	
    CLONE(CMeshRender);
public:
	Ptr<CMaterial> GetCurrentMaterial() { return current_material_; }
	Ptr<CMesh> GetMesh() { return mesh_; }

    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetCloneMaterial();

    void SetMaterial(Ptr<CMaterial> material);
    void SetMesh(Ptr<CMesh> mesh) { mesh_ = mesh; }

public:
    void SaveToScene(FILE* file) override;
    void LoadFromScene(FILE* file) override;
private:
    Ptr<CMaterial> current_material_;
    Ptr<CMaterial> shared_material_;
    Ptr<CMaterial> clone_material_;
    Ptr<CMesh> mesh_;
};

