#include "pch.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CComponent.h"
#include "CMeshRender.h"

CGameObject::CGameObject()
	:m_components{}
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::Update()
{
	for (size_t i = 0; i < m_components.size(); ++i)
	{
		if(nullptr != m_components[i])
			m_components[i]->Update();
	} 
}

void CGameObject::Render()
{
	if (nullptr == MeshRender())
		return;
	MeshRender()->Render();
}

void CGameObject::AddComponent(CComponent* _comp)
{
	UINT _type = ENUM_TO_NUMBER(_comp->GetType());
	assert(!m_components[_type]);
	_comp->m_owner = this;
	m_components[_type] = _comp;
	
}
