#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CShader.h"
#include "CTexture.h"
#include "CGameObject.h"
#include "CTransform.h"
CMeshRender::CMeshRender()
	:CComponent(COMPONENT_TYPE::MESHRENDER)
	,m_mesh(nullptr)
	,m_shader(nullptr)
	,m_texture(nullptr)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::Render()
{
	if (nullptr == m_shader || nullptr == m_mesh)
	{
		return;
	}
	GetOwner()->Transform()->UpdateData();
	
	m_shader->UpdateData();

	if (nullptr != m_texture)
	{
		m_texture->SetPipelineStage(PS_PIXEL, 0);
		m_texture->UpdateData();
	}

	m_mesh->Render();
}
