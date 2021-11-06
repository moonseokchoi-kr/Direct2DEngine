#include "pch.h"
#include "CParticleSystem.h"

#include "CResourceManager.h"
#include "CParticleUpdateShader.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	:CComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, particle_buffer_(nullptr)
	, max_particle_count_(5)
{
	particle_mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"RectMesh");
	particle_material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"particle_material");
	particle_update_shader_ = static_cast<CParticleUpdateShader*>(CResourceManager::GetInst()->FindRes<CComputeShader>(L"particle_update_shader").Get());

	particle_buffer_ = new CStructuredBuffer;
	particle_buffer_->Create(sizeof(Particle), max_particle_count_, STRUCTURE_BUFFER_TYPE::READ_WRITE, nullptr);

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
	particle_update_shader_->SetParticleBuffer(particle_buffer_);
	particle_update_shader_->Excute();
}

void CParticleSystem::FinalUpdate()
{
	UpdateData();

	particle_mesh_->Render();
}

void CParticleSystem::Render()
{
	GetTransform()->UpdateData();
	particle_mesh_->UpdateData();
	particle_material_->UpdateData();

	particle_buffer_->UpdateData(PIPELINE_STAGE::PS_VERTEX, 13);
}
