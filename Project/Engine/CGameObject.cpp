#include "pch.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CComponent.h"
#include "CMeshRender.h"

CGameObject::CGameObject()
	:components_{}
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::Update()
{
	for (size_t i = 0; i < components_.size(); ++i)
	{
		if(nullptr != components_[i])
			components_[i]->Update();
	} 
}

void CGameObject::Render()
{
	if (nullptr == MeshRender())
		return;
	MeshRender()->Render();
}

void CGameObject::AddComponent(CComponent* comp)
{
	UINT type = ENUM_TO_NUMBER(comp->GetType());
	assert(!components_[type]);
	comp->m_owner = this;
	components_[type] = comp;
	
}