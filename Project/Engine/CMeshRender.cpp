#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CShader.h"

#include "CGameObject.h"
#include "CTransform.h"

#include "CMaterial.h"
#include "ptr.h"

CMeshRender::CMeshRender()
	:CComponent(COMPONENT_TYPE::MESHRENDER)
	,mesh_(nullptr)
	,material_(nullptr)

{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::Render()
{
	if (nullptr == material_ || nullptr == material_->GetShader() || nullptr == mesh_)
	{
		return;
	}

	GetOwner()->Transform()->UpdateData();
	
	material_->UpdateData();
	

	mesh_->Render();
}

void CMeshRender::FinalUpdate()
{
}
