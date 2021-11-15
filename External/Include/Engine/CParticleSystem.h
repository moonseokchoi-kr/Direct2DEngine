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
    
    void Clear();
public:
    CLONE(CParticleSystem);

private:
    Ptr<CMesh>  particle_mesh_;
    Ptr<CMaterial> particle_material_;
    Ptr<CParticleUpdateShader> particle_update_shader_;
    CStructuredBuffer* particle_buffer_;

    UINT    max_particle_count_;
};

