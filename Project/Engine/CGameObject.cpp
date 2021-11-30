#include "pch.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CComponent.h"
#include "CMeshRender.h"
#include "CCollider2D.h"
#include "CParticleSystem.h"
#include "CScript.h"


#include "CSceneManager.h"
#include "CScene.h"
#include "CResourceManager.h"
#include "CTimeManager.h"

#include "CLayer.h"

CGameObject::CGameObject()
	:component_array_{}
	,parent_object_(nullptr)
	,layer_index_(-1)
	,object_state_(OBJECT_STATE::ALIVE)
{
}

CGameObject::CGameObject(const CGameObject& origin)
	:CEntity(origin)
	, component_array_{}
	, parent_object_(nullptr)
	, layer_index_(-1)
	, object_state_(OBJECT_STATE::ALIVE)
{
	for (CComponent* component : origin.component_array_)
	{
		if (nullptr != component)
		{
			AddComponent(component->Clone());
		}
		
	}

	for (CScript* script : origin.scripts_vector_)
	{
		scripts_vector_.push_back(script->Clone());
	}

	for (CGameObject* child : origin.child_object_vector_)
	{
		AddChild(child->Clone());
	}
}

CGameObject::~CGameObject()
{
	Safe_Delete_Array(component_array_);
	Safe_Delete_Vec(child_object_vector_);
	Safe_Delete_Vec(scripts_vector_);
}

void CGameObject::Start()
{
	for (CComponent* component : component_array_)
	{
		if (nullptr != component)
			component->Start();
	}

	for (const auto& script : scripts_vector_)
	{
		script->Start();
	}

	for (CGameObject* child : child_object_vector_)
	{
		child->Start();
	}
}

void CGameObject::Update()
{
	if (CheckDead())
		return;
	for (CComponent* component : component_array_)
	{
		if(nullptr != component)
			component->Update();
	} 
	for (const auto& script : scripts_vector_)
	{
		script->Update();
	}

	for (CGameObject* child : child_object_vector_)
	{
			child->Update();
	}
}

void CGameObject::LateUpdate()
{
	if (OBJECT_STATE::DEAD == object_state_)
		return;

	for (CComponent* component : component_array_)
	{
		if (nullptr != component)
			component->LateUpdate();
	}
	for (const auto& script : scripts_vector_)
	{
		script->LateUpdate();
	}

	for (CGameObject* child : child_object_vector_)
	{
		child->LateUpdate();
	}
}

void CGameObject::FinalUpdate()
{
	for (CComponent* component : component_array_)
	{
		if (nullptr != component)
			component->FinalUpdate();
	}
	for (const auto& script : scripts_vector_)
	{
		script->FinalUpdate();
	}

	for (CGameObject* child : child_object_vector_)
	{
		child->FinalUpdate();
	}

	CScene* currentScene = CSceneManager::GetInst()->GetCurrentScene();
	CLayer* currentLayer = currentScene->GetLayer(layer_index_);
	currentLayer->RegisterObject(this);
}

void CGameObject::Render()
{
	if (nullptr != MeshRender())
	{
		MeshRender()->Render();
	}

	if (nullptr != ParticleSystem())
	{
		ParticleSystem()->Render();
	}
	if (nullptr != Collider2D())
	{
		Collider2D()->Render();
	}

}

void CGameObject::AddComponent(CComponent* comp)
{
	COMPONENT_TYPE type = comp->GetType();
	if (COMPONENT_TYPE::SCRIPT == type)
	{
		scripts_vector_.push_back(dynamic_cast<CScript*>(comp));
		comp->owner_ = this;
		return;
	}
	const UINT index = ENUM_TO_NUMBER(type);

	assert(!component_array_[index]);

	comp->owner_ = this;
	component_array_[index] = comp;
	
}

void CGameObject::AddChild(CGameObject* child)
{
	child_object_vector_.push_back(child);
	child->parent_object_ = this;
}



bool CGameObject::CheckDead()
{
	if (OBJECT_STATE::RESERVE_DEAD == object_state_)
	{
		if (IsDead())
			return true;
		delay_accumulated_time += fDT;
		if (dead_time <= delay_accumulated_time)
		{
			object_state_ = OBJECT_STATE::DEAD;
			return true;
		}

	}
	return false;
}
#include "CPrefab.h"
void CGameObject::ReigsterAsPrefab(const wstring& prefabName)
{
	if (CResourceManager::GetInst()->FindRes<CPrefab>(prefabName).Get())
	{
		assert(nullptr);
	}

	CResourceManager::GetInst()->AddPrefab(prefabName, this);
}