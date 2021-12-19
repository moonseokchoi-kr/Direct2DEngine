#pragma once
#include "CComponent.h"
#include "ptr.h"
#include "CMesh.h"
#include "CMaterial.h"

class CStructuredBuffer;
class CParticleUpdateShader;

class CParticleSystem :
    public CComponent
{
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& origin);
    ~CParticleSystem();
public:
    void Start() override;
    void UpdateData() override;
    void FinalUpdate() override;

    void Render();
    
    void Clear();
public:
    Ptr<CMesh> GetMesh() { return particle_mesh_; }
    Ptr<CMaterial> GetMaterial() { return particle_material_; }
public:
    CLONE(CParticleSystem);


    void SaveToScene(FILE* file) override;
    void LoadFromScene(FILE* file) override;
private:
    Ptr<CMesh>  particle_mesh_;
    Ptr<CMaterial> particle_material_;
    
    Ptr<CParticleUpdateShader> particle_update_shader_;

    Ptr<CTexture> particle_texture_;

    CStructuredBuffer* particle_buffer_;
    CStructuredBuffer* particle_shared_data_buffer_;

    UINT    max_particle_count_;
    Vec3    spawn_range_;
    Vec3    min_scale_;
    Vec3    max_scale_;
    Vec4    start_color_;
    Vec4    end_color_;
    
    float   particle_min_life_;
    float   particle_max_life_;

    float   min_speed_;
    float   max_speed_;

    float   particle_spawn_frequency_;

    float   accumulate_time_;
};

