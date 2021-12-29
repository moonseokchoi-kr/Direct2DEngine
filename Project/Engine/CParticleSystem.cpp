#include "pch.h"
#include "CParticleSystem.h"

#include "CResourceManager.h"
#include "CParticleUpdateShader.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	:CComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, particle_buffer_(nullptr)
	, accumulate_time_(0.f)
	, activable_count_(8)
{
	particle_mesh_ = CResourceManager::GetInst()->FindRes<CMesh>(L"PointMesh");
	particle_material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"particle_material");
	particle_update_shader_ = static_cast<CParticleUpdateShader*>(CResourceManager::GetInst()->FindRes<CComputeShader>(L"particle_update_shader").Get());
	particle_texture_ = CResourceManager::GetInst()->FindRes<CTexture>(L"particle_01");

	particle_buffer_ = new CStructuredBuffer;
	particle_shared_data_buffer_ = new CStructuredBuffer;
	
	data_.max_particle_count = 10000;
	data_.min_scale = Vec3(5.f, 5.f, 0.f);
	data_.max_scale = Vec3(15.f, 15.f, 0.f);
	data_.max_speed= 80.f;
	data_.min_speed = 30.f;
	data_.start_color = Vec4(0.72f, 0.72f, 0.72f, 1.f);
	data_.end_color = Vec4(1.f, 1.f, 1.f, 1.f);
	data_.particle_max_life = 10.f;
	data_.particle_min_life = 5.f;
	data_.particle_spawn_frequency = 0.15f;
	data_.spawn_range=Vec3(3000, 3000, 0);

	

	particle_buffer_->Create(sizeof(Particle), data_.max_particle_count, STRUCTURE_BUFFER_TYPE::READ_WRITE,nullptr,false);
	particle_shared_data_buffer_->Create(sizeof(ParticleSharedData), 1, STRUCTURE_BUFFER_TYPE::READ_WRITE, nullptr, true);



}

CParticleSystem::CParticleSystem(const CParticleSystem& origin)
	:CComponent(origin)
	,particle_buffer_(nullptr)
	,particle_shared_data_buffer_(nullptr)
	,particle_material_(origin.particle_material_)
	,particle_texture_(origin.particle_texture_)
	,particle_update_shader_(origin.particle_update_shader_)
	, accumulate_time_(0.f)
	, activable_count_(origin.activable_count_)
	,data_(origin.data_)
{
	particle_buffer_->Create(sizeof(Particle), data_.max_particle_count, STRUCTURE_BUFFER_TYPE::READ_WRITE, nullptr, false);
	particle_shared_data_buffer_->Create(sizeof(ParticleSharedData), 1, STRUCTURE_BUFFER_TYPE::READ_WRITE, nullptr, true);
}

CParticleSystem::~CParticleSystem()
{
	SafeDelete(particle_buffer_);
	SafeDelete(particle_shared_data_buffer_);
}

void CParticleSystem::Start()
{
	particle_buffer_->Create(particle_buffer_->GetElementSize(), particle_buffer_->GetElementCount(), particle_buffer_->GetBufferType(), nullptr,particle_buffer_->IsCpuAccess());
	particle_shared_data_buffer_->Create(particle_shared_data_buffer_->GetElementSize(), particle_shared_data_buffer_->GetElementCount(), 
		particle_shared_data_buffer_->GetBufferType(), nullptr, particle_shared_data_buffer_->IsCpuAccess());
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
	if (data_.particle_spawn_frequency < accumulate_time_)
	{
		accumulate_time_ = 0.f;
		ParticleSharedData sharedData = {};
		sharedData.activable_count = activable_count_;
		particle_shared_data_buffer_->SetData(&sharedData, sizeof(ParticleSharedData));
	}
	particle_update_shader_->SetParticleBuffer(particle_buffer_);
	particle_update_shader_->SetParticleSharedDataBuffer(particle_shared_data_buffer_);

	particle_update_shader_->SetObjectPosition(GetTransform()->GetWorldPos());
	particle_update_shader_->SetSpawnRange(data_.spawn_range);

	particle_update_shader_->SetMinScale(data_.min_scale);
	particle_update_shader_->SetMaxScale(data_.max_scale);

	particle_update_shader_->SetStartColor(data_.start_color);
	particle_update_shader_->SetEndColor(data_.end_color);

	particle_update_shader_->SetMinSpeed(data_.min_speed);
	particle_update_shader_->SetMaxSpeed(data_.max_speed);

	particle_update_shader_->SetMinLife(data_.particle_min_life);
	particle_update_shader_->SetMaxLife(data_.particle_max_life);

	particle_update_shader_->Excute();
}

void CParticleSystem::Render()
{
	UpdateData();
	particle_material_->UpdateData();
	particle_mesh_->RenderParticle(data_.max_particle_count);
	Clear();
}

void CParticleSystem::Clear()
{
	particle_buffer_->Clear();
}

void CParticleSystem::SaveToScene(FILE* file)
{
	CComponent::SaveToScene(file);

	SaveResReference(particle_texture_, file);
	fwrite(&data_, sizeof(ParticleData), 1, file);
	fwrite(&activable_count_, sizeof(float), 1, file);
}

void CParticleSystem::LoadFromScene(FILE* file)
{
	CComponent::LoadFromScene(file);

	LoadResReference(particle_texture_, file);

	fread(&data_, sizeof(ParticleData), 1, file);
	fread(&activable_count_, sizeof(float), 1, file);
}
