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
    void UpdateData() override;
    void FinalUpdate() override;

    void Render();
    
public:
    CLONE(CParticleSystem);

private:
    Ptr<CMesh>  particle_mesh_;
    Ptr<CMaterial> particle_material_;
    Ptr<CParticleUpdateShader> particle_update_shader_;
    CStructuredBuffer* particle_buffer_;
    CStructuredBuffer* particle_shared_data_buffer_;

    UINT    max_particle_count_;

    Vec3 spawn_range_;
    Vec3 start_scale_;
    Vec3 end_scale_;
    Vec3 start_color_;
    Vec3 end_color_;

    float particle_min_life_;
    float particle_max_life_;

    float spawn_prequency_;
    float accumlated_time_;
};

