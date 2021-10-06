#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CShader.h"
#include "CTexture.h"
#include "CGameObject.h"
#include "CTransform.h"
CMeshRender::CMeshRender()
	:CComponent(COMPONENT_TYPE::MESHRENDER)
	,mesh_(nullptr)
	,shader_(nullptr)
	,texture_(nullptr)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::Render()
{
	if (nullptr == shader_ || nullptr == mesh_)
	{
		return;
	}
	GetOwner()->Transform()->UpdateData();
	
	shader_->UpdateData();

	if (nullptr != texture_)
	{
		texture_->SetPipelineStage(PS_PIXEL, 0);
		texture_->UpdateData();
	}

	mesh_->Render();
}
