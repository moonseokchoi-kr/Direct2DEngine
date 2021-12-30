#include "pch.h"
#include "ParticleWidget.h"
#include "DataInputWidget.h"

#include <Engine/CParticleSystem.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>
#include <Engine/CParticleUpdateShader.h>
#include <Engine/CTexture.h>

#include <Engine/CResourceManager.h>

ParticleWidget::ParticleWidget()
	:ComponentWidget("Particle System", Vec2(0,340))
{
	material_combo_.SetName("##material");
	mesh_combo_.SetName("##mesh");
	texture_combo_.SetName("##texture");
	data_tree_.SetName("##data_tree");
	data_tree_.AddItem(nullptr, "data", 0);
}

ParticleWidget::~ParticleWidget()
{
}

void ParticleWidget::Update()
{
	CParticleSystem* particle = GetTarget()->ParticleSystem();
	Ptr<CMaterial> material = particle->GetMaterial();
	Ptr<CMesh> mesh = particle->GetMesh();
	Ptr<CTexture> texture = particle->GetParticleTexture();
	ParticleData particleData = particle->GetParticleData();
	float activableCount = particle->GetActivableCount();
	Start();
	if (ImGui::CollapsingHeader(GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginTable("##particle", 2, ImGuiTableFlags_Resizable))
		{

			ImGui::TableNextColumn();
			ImGui::Text("Mesh");
			ImGui::TableNextColumn();

			const unordered_map<wstring, CResource*>& meshMap = CResourceManager::GetInst()->GetResource<CMesh>();
			int count = 0;
			mesh_combo_.SetCallback(this, (COMBO_CALLBACK)&ParticleWidget::ChangeMesh);
			for (const auto& pair : meshMap)
			{
				mesh_combo_.AddComboData(WStringToString(pair.first));
				if (nullptr != mesh && pair.first == mesh->GetKey())
					mesh_combo_.SetCurrentIndex(count);
				++count;
			}
			mesh_combo_.Update();


			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::Text("Material");
			ImGui::TableNextColumn();

			const unordered_map<wstring, CResource*>& materialMap = CResourceManager::GetInst()->GetResource<CMaterial>();
			count = 0;
			material_combo_.SetCallback(this, (COMBO_CALLBACK)&ParticleWidget::ChangeMaterial);
			for (const auto& pair : materialMap)
			{
				material_combo_.AddComboData(WStringToString(pair.first));
				if (nullptr != material && pair.first == material->GetKey())
					material_combo_.SetCurrentIndex(count);
				++count;
			}

			material_combo_.Update();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::Text("Texture");
			ImGui::TableNextColumn();

			const unordered_map<wstring, CResource*>& textureMap = CResourceManager::GetInst()->GetResource<CTexture>();
			count = 0;
			texture_combo_.SetCallback(this, (COMBO_CALLBACK)&ParticleWidget::ChangeTexture);
			for (const auto& pair : textureMap)
			{
				texture_combo_.AddComboData(WStringToString(pair.first));
				if (nullptr != texture && pair.first == texture->GetKey())
					texture_combo_.SetCurrentIndex(count);
				++count;
			}

			texture_combo_.Update();
			
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::Text("Activable Count");
			
			ImGui::TableNextColumn();
			if (DataInputWidget::DataInputFloat(L"count", &activableCount))
			{
				particle->SetActivableCount(activableCount);
			}
			ImGui::EndTable();

			ImGui::Spacing();

			if(ImGui::CollapsingHeader("Particle Data"))
			{
				ImGui::Indent();

				if (ImGui::BeginTable("##particle_Data",2, ImGuiTableFlags_Resizable))
				{
					ImGui::TableNextColumn();
					ImGui::Text("Start Color");
					ImGui::TableNextColumn();

					DataInputWidget::DataInputVec4(L"Start Color", particleData.start_color);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();


					ImGui::Text("End Color");
					ImGui::TableNextColumn();
					DataInputWidget::DataDragInputVec4(L"End Color", particleData.end_color);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();


					//Vec3    spawn_range;

					ImGui::Text("Spawn Range");
					ImGui::TableNextColumn();
					DataInputWidget::DataDragInputVec3(L"Spawn Range", particleData.spawn_range);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();


					//Vec3    min_scale;

					ImGui::Text("Min Scale");
					ImGui::TableNextColumn();
					DataInputWidget::DataDragInputVec3(L"Min Scale", particleData.min_scale);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					//Vec3    max_scale;

					ImGui::Text("Max Scale");
					ImGui::TableNextColumn();
					DataInputWidget::DataDragInputVec3(L"Max Scale", particleData.max_scale);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					//UINT    max_particle_count;

					ImGui::Text("Max Count");
					ImGui::TableNextColumn();
					DataInputWidget::DataDragInputInt(L"Max Count", &(particleData.max_particle_count));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					//float   particle_min_life;

					ImGui::Text("Min Life Time");
					ImGui::TableNextColumn();
					DataInputWidget::DataDragInputFloat(L"Min Life Time", &(particleData.particle_min_life));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					//float   particle_max_life;

					ImGui::Text("Max Life Time");
					ImGui::TableNextColumn();
					DataInputWidget::DataDragInputFloat(L"Max Life Time", &(particleData.particle_max_life));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					//float   min_speed;

					ImGui::Text("Min Speed");
					ImGui::TableNextColumn();
					DataInputWidget::DataDragInputFloat(L"Min Speed", &(particleData.min_speed));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					//float   max_speed;

					ImGui::Text("Start Color");
					ImGui::TableNextColumn();
					DataInputWidget::DataDragInputFloat(L"Max Speed", &(particleData.max_speed));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();

					//float   particle_spawn_frequency;;
					ImGui::Text("Spawn Frequency");
					ImGui::TableNextColumn();
					DataInputWidget::DataDragInputFloat(L"Spawn Frequency", &(particleData.particle_spawn_frequency));

					particle->SetParticleData(particleData);
					ImGui::EndTable();
				}

			}
		}
	}
	End();
}

void ParticleWidget::ChangeMesh(DWORD_PTR instance, DWORD_PTR meshName)
{
	ComboWidget* widget = (ComboWidget*)instance;
	string strName = widget->GetSelectedItem();
	CParticleSystem* particle = GetTarget()->ParticleSystem();
	particle->SetMesh(CResourceManager::GetInst()->FindRes<CMesh>(StringToWString(strName)));
}

void ParticleWidget::ChangeMaterial(DWORD_PTR instance, DWORD_PTR materialName)
{
	ComboWidget* widget = (ComboWidget*)instance;
	string strName = widget->GetSelectedItem();
	CParticleSystem* particle = GetTarget()->ParticleSystem();
	particle->SetMaterial(CResourceManager::GetInst()->FindRes<CMaterial>(StringToWString(strName)));
}

void ParticleWidget::ChangeTexture(DWORD_PTR instance, DWORD_PTR textureName)
{
	ComboWidget* widget = (ComboWidget*)instance;
	string strName = widget->GetSelectedItem();
	CParticleSystem* particle = GetTarget()->ParticleSystem();
	particle->SetTexture(CResourceManager::GetInst()->FindRes<CTexture>(StringToWString(strName)));
}

