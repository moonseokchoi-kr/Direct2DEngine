#pragma once
#include "CComponent.h"
#include "ptr.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CParticleUpdateShader.h"

struct ParticleData
{
	Vec4    start_color;
	Vec4    end_color;

	Vec3    spawn_range;
	Vec3    min_scale;
	Vec3    max_scale;


	int    max_particle_count;
	float   particle_min_life;
	float   particle_max_life;

	float   min_speed;
	float   max_speed;

	float   particle_spawn_frequency;

};

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

    Ptr<CParticleUpdateShader> GetUpdateShader() { return particle_update_shader_; }

    Ptr<CTexture> GetParticleTexture() { return particle_texture_; }
    ParticleData& GetParticleData() { return data_; }
    float GetActivableCount() { return activable_count_; }

    void SetParticleData(const ParticleData& pd) { data_ = pd; }
    void SetActivableCount(float count) { activable_count_ = count; }
    void SetTexture(Ptr<CTexture> texture) { particle_texture_ = texture; }
    void SetUpdateShader(Ptr<CParticleUpdateShader> updateShader) { particle_update_shader_ = updateShader; }
    void SetMaterial(Ptr<CMaterial> material) { particle_material_ = material; }
    void SetMesh(Ptr<CMesh> mesh) { particle_mesh_ = mesh; }
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

    ParticleData data_;

    float   activable_count_;
	float   accumulate_time_;
};

