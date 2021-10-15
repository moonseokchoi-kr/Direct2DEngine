#include "pch.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CComponent.h"
#include "CMeshRender.h"

#include "CSceneManager.h"
#include "CScene.h"

#include "CLayer.h"

CGameObject::CGameObject()
	:component_array_{}
	,parent_object_(nullptr)
	,layer_index_(-1)
{
}

CGameObject::CGameObject(const CGameObject& origin)
	:parent_object_(nullptr)
	,layer_index_(-1)
{
	for (CComponent* component : origin.component_array_)
	{
		if (nullptr != component)
		{
			AddComponent(component->Clone());
		}
		
	}

	for (CGameObject* child : origin.child_object_vector_)
	{
		AddChild(child->Clone());
	}
}

CGameObject::~CGameObject()
{
}

void CGameObject::Update()
{
	for (CComponent* component : component_array_)
	{
		if(nullptr != component)
			component->Update();
	} 

	for (CGameObject* child : child_object_vector_)
	{
			child->Update();
	}
}

void CGameObject::LateUpdate()
{
	for (CComponent* component : component_array_)
	{
		if (nullptr != component)
			component->LateUpdate();
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
	if (nullptr == MeshRender())
		return;
	MeshRender()->Render();
}

void CGameObject::AddComponent(CComponent* comp)
{
	const UINT type = ENUM_TO_NUMBER(comp->GetType());
	assert(!component_array_[type]);
	comp->owner_ = this;
	component_array_[type] = comp;
	
}

void CGameObject::AddChild(CGameObject* child)
{
	child_object_vector_.push_back(child);
	child->parent_object_ = this;
}
