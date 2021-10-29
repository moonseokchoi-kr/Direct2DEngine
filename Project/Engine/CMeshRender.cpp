#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CGraphicsShader.h"

#include "CGameObject.h"
#include "CTransform.h"

#include "CMaterial.h"
#include "ptr.h"

CMeshRender::CMeshRender()
	:CComponent(COMPONENT_TYPE::MESHRENDER)
	,mesh_(nullptr)
	,current_material_(nullptr)

{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::Render()
{
	if (nullptr == current_material_ || nullptr == current_material_->GetShader() || nullptr == mesh_)
	{
		return;
	}

	GetOwner()->Transform()->UpdateData();
	
	current_material_->UpdateData();
	

	mesh_->Render();
}

void CMeshRender::FinalUpdate()
{
}

Ptr<CMaterial> CMeshRender::GetSharedMaterial()
{
	current_material_ = shared_material_;
	return current_material_;
}

Ptr<CMaterial> CMeshRender::GetCloneMaterial()
{
	if (nullptr != shared_material_)
	{
		if (nullptr != clone_material_)
		{
			clone_material_ = shared_material_->Clone();
		}
		current_material_ = clone_material_;
	}
	else
	{
		return nullptr;
	}
	return current_material_;
}

void CMeshRender::SetMaterial(Ptr<CMaterial> material)
{
	shared_material_ = material;
	current_material_ = shared_material_;

	if (nullptr != shared_material_)
	{
		delete clone_material_.Get();
		clone_material_ = nullptr;
	}
}
