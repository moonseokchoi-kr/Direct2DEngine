#include "pch.h"
#include "CParticleSystem.h"

#include "CResourceManager.h"
#include "CParticleUpdateShader.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	:CComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, particle_buffer_(nullptr)
	, particle_shared_data_buffer_(nullptr)
	, max_particle_count_(100)
	, accumlated_time_(0)
	, start_scale_(Vec3(1.f, 1.f, 0.f))
	, end_scale_(Vec3(50.f, 50.f, 0.f))
	, spawn_range_(Vec3(500.f, 500.f, 0.f))
	, particle_min_life_(2.f)
	, particle_max_life_(5.f)
	, spawn_prequency_(0.1f)
	
{
	particle_mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"PointMesh");
	particle_material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"particle_material");
	particle_update_shader_ = static_cast<CParticleUpdateShader*>(CResourceManager::GetInst()->FindRes<CComputeShader>(L"particle_update_shader").Get());

	particle_buffer_ = new CStructuredBuffer;


	particle_buffer_->Create(sizeof(Particle), max_particle_count_, STRUCTURE_BUFFER_TYPE::READ_WRITE,nullptr,false);

	particle_shared_data_buffer_ = new CStructuredBuffer;

	particle_shared_data_buffer_->Create(sizeof(ParticleSharedData), 1, STRUCTURE_BUFFER_TYPE::READ_WRITE, nullptr,true);

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
	particle_buffer_->UpdateData(PIPELINE_STAGE::PS_VERTEX, 14);
}

void CParticleSystem::FinalUpdate()
{
	accumlated_time_ += fDT;
	if (spawn_prequency_ < accumlated_time_)
	{
		accumlated_time_ = 0;

		ParticleSharedData shared = {};
		shared.activable_count = 1;
		particle_shared_data_buffer_->SetData(&shared, sizeof(ParticleSharedData));
	}
	particle_update_shader_->SetParticleBuffer(particle_buffer_);
	particle_update_shader_->SetParticleSharedBuffer(particle_shared_data_buffer_);
	particle_update_shader_->SetObjectPos(GetTransform()->GetWorldPos());
	particle_update_shader_->SetSpawnRange(spawn_range_);
	particle_update_shader_->SetStartScale(start_scale_);
	particle_update_shader_->SetEndScale(end_scale_);

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
