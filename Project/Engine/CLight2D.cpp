#include "pch.h"
#include "CLight2D.h"

#include "CTransform.h"

#include "CRenderManager.h"


CLight2D::CLight2D()
	:CComponent(COMPONENT_TYPE::LIGHT2D)
	,light_index_(-1)
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::FinalUpdate()
{
	light_info_.light_direction = GetTransform()->GetWorldPos();
	light_index_ = CRenderManager::GetInst()->RegisterLight2D(this);
}

void CLight2D::UpdateData()
{
}
