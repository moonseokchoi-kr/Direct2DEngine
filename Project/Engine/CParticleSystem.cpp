#include "pch.h"
#include "CParticleSystem.h"

#include "CResourceManager.h"
#include "CParticleUpdateShader.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	:CComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, particle_buffer_(nullptr)
	, max_particle_count_(10000)
	, spawn_range_(Vec3(2000.f,2000.f,500.f))
	, min_scale_(Vec3(5.f,5.f,0.f))
	, max_scale_(Vec3(15.f,15.f,0.f))
	, max_speed_(80.f)
	, min_speed_(30.f)
	, start_color_(Vec4(0.72f, 0.72f, 0.72f,1.f))
	, end_color_(Vec4(1.f, 1.f, 1.f,1.f))
	, particle_min_life_(5.f)
	, particle_max_life_(10.f)
	, particle_spawn_frequency_(0.15f)
	, accumulate_time_(0.f)
{
	particle_mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"PointMesh");
	particle_material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"particle_material");
	particle_update_shader_ = static_cast<CParticleUpdateShader*>(CResourceManager::GetInst()->FindRes<CComputeShader>(L"particle_update_shader").Get());
	particle_texture_ = CResourceManager::GetInst()->FindRes<CTexture>(L"particle_01");
	particle_buffer_ = new CStructuredBuffer;
	particle_shared_data_buffer_ = new CStructuredBuffer;


	particle_buffer_->Create(sizeof(Particle), max_particle_count_, STRUCTURE_BUFFER_TYPE::READ_WRITE,nullptr,false);
	particle_shared_data_buffer_->Create(sizeof(ParticleShadredData), 1, STRUCTURE_BUFFER_TYPE::READ_WRITE, nullptr, true);


}

CParticleSystem::CParticleSystem(const CParticleSystem& origin)
	:CComponent(origin)
{
}

CParticleSystem::~CParticleSystem()
{
	SafeDelete(particle_buffer_);
	SafeDelete(particle_shared_data_buffer_);
}

void CParticleSystem::UpdateData()
{
	GetTransform()->UpdateData();
	particle_buffer_->UpdateData(PS_VERTEX|PS_GEOMETRY|PS_PIXEL, 14);
	particle_material_->SetData(SHADER_PARAM::TEX_0, particle_texture_.Get());
}

void CParticleSystem::FinalUpdate()
{
	accumulate_time_ += fDT;
	if (particle_spawn_frequency_ < accumulate_time_)
	{
		accumulate_time_ = 0.f;
		ParticleShadredData shared = {};
		shared.activable_count = 8;
		particle_shared_data_buffer_->SetData(&shared, sizeof(ParticleShadredData));
	}
	particle_update_shader_->SetParticleBuffer(particle_buffer_);
	particle_update_shader_->SetParticleSharedDataBuffer(particle_shared_data_buffer_);

	particle_update_shader_->SetObjectPosition(GetTransform()->GetWorldPos());
	particle_update_shader_->SetSpawnRange(spawn_range_);

	particle_update_shader_->SetMinScale(min_scale_);
	particle_update_shader_->SetMaxScale(max_scale_);

	particle_update_shader_->SetStartColor(start_color_);
	particle_update_shader_->SetEndColor(end_color_);

	particle_update_shader_->SetMinSpeed(min_speed_);
	particle_update_shader_->SetMaxSpeed(max_speed_);

	particle_update_shader_->SetMinLife(particle_min_life_);
	particle_update_shader_->SetMaxLife(particle_max_life_);

	particle_update_shader_->Excute();
}

void CParticleSystem::Render()
{
	UpdateData();
	particle_material_->UpdateData();
	particle_mesh_->RenderParticle(max_particle_count_);
	Clear();
}

void CParticleSystem::Clear()
{
	particle_buffer_->Clear();
}
