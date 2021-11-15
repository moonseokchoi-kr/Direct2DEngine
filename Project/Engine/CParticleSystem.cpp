#include "pch.h"
#include "CParticleSystem.h"

#include "CResourceManager.h"
#include "CParticleUpdateShader.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	:CComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, particle_buffer_(nullptr)
	, max_particle_count_(100)
{
	particle_mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"PointMesh");
	particle_material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"particle_material");
	particle_update_shader_ = static_cast<CParticleUpdateShader*>(CResourceManager::GetInst()->FindRes<CComputeShader>(L"particle_update_shader").Get());

	particle_buffer_ = new CStructuredBuffer;

	array<Particle, 100> particleArray = { };

	float dist = 15.f;
	float view_scale = 10.f;

	for (int i = 0; i < max_particle_count_; ++i)
	{
		particleArray[i].world_position = Vec3(((float)max_particle_count_ / 2.f) * -dist + (float)i * dist, 0.f, 100.f);
		particleArray[i].view_scale = Vec3(view_scale, view_scale, 1.f);
		particleArray[i].is_active = true;
	}

	particle_buffer_->Create(sizeof(Particle), max_particle_count_, STRUCTURE_BUFFER_TYPE::READ_WRITE, particleArray.data());

}

CParticleSystem::CParticleSystem(const CParticleSystem& origin)
	:CComponent(origin)
{
}

CParticleSystem::~CParticleSystem()
{
	SafeDelete(particle_buffer_);
}

void CParticleSystem::UpdateData()
{
	GetTransform()->UpdateData();
	particle_buffer_->UpdateData(PS_VERTEX|PS_GEOMETRY|PS_PIXEL, 13);
}

void CParticleSystem::FinalUpdate()
{
	particle_update_shader_->SetParticleBuffer(particle_buffer_);
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
