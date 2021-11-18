#include "pch.h"
#include "CCamera.h"


#include "CTransform.h"

#include "CDevice.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CRenderManager.h"

#include "CMeshRender.h"
#include "CParticleSystem.h"
#include "CMaterial.h"

#include "CLayer.h"
#include "CScene.h"

CCamera::CCamera()
	:CComponent(COMPONENT_TYPE::CAMERA)
	, angle_of_view_y_(XM_PI/2.f)
	, far_z_(10000.f)
	, projection_type_(PROJECTION_TYPE::PERSPECTIVE)
	, camera_index_(-1)
{
}

CCamera::~CCamera()
{
}


void CCamera::Update()
{
// 	Vec3 pos = GetTransform()->GetPosition();
// 	if(KEY_HOLD(KEY::W))
// 	{
// 		pos.y -= 100.f * fDT;
// 	}
// 	if (KEY_HOLD(KEY::A))
// 	{
// 		pos.x -= 100.f * fDT;
// 	}
// 	if (KEY_HOLD(KEY::S))
// 	{
// 		pos.y += 100.f * fDT;
// 	}
// 	if (KEY_HOLD(KEY::D))
// 	{
// 		pos.x += 100.f * fDT;
// 	}
// 	GetTransform()->SetPosition(pos);

}

void CCamera::LateUpdate()
{
	
}

void CCamera::FinalUpdate()
{
	const Vec3 pos = GetTransform()->GetPosition();
	const Vec2 resolution = CDevice::GetInst()->GetResolution();
	view_matrix_ = XMMatrixTranslation(pos.x, pos.y, pos.z);
	view_matrix_ = view_matrix_.Invert();


	if(PROJECTION_TYPE::PERSPECTIVE == projection_type_)
	{
		projection_matrix_ = XMMatrixPerspectiveFovLH(angle_of_view_y_, CDevice::GetInst()->GetAspectRatio(), 1.f, far_z_);
	}
	else if(PROJECTION_TYPE::ORTHO == projection_type_)
	{
		projection_matrix_ = XMMatrixOrthographicLH(resolution.x, resolution.y, 1.f, far_z_);
	}

	camera_index_ = CRenderManager::GetInst()->RegisterCamera(this, camera_index_);
}

void CCamera::Render()
{
	g_transform.view_matrix = view_matrix_;
	g_transform.projection_matrix = projection_matrix_;

	CScene* currentScene = CSceneManager::GetInst()->GetCurrentScene();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (layer_check_out_ & (1 << i))
		{
			CLayer* layer = currentScene->GetLayer(i);
			layer->Render();
		}
	}
}

void CCamera::SeperateRenderObject()
{
	forward_render_object_vector_.clear();
	particle_object_vector_.clear();
	post_effect_vector_.clear();

	CScene* currentScene = CSceneManager::GetInst()->GetCurrentScene();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (layer_check_out_ & (1 << i))
		{
			CLayer* layer = currentScene->GetLayer(i);

			const vector<CGameObject*>& objectVector = layer->GetLayerObjects();

			RENDER_TIMING rt = RENDER_TIMING::NONE;

			for(CGameObject* object : objectVector)
			{
				if (object->MeshRender())
				{
					if (nullptr == object->MeshRender()->GetMesh() || nullptr == object->MeshRender()->GetSharedMaterial() || nullptr == object->MeshRender()->GetSharedMaterial()->GetShader())
					{
						continue;
					}

					Ptr<CGraphicsShader> shader = object->MeshRender()->GetSharedMaterial()->GetShader();
					rt = shader->GetRenderTiming();
				}
				else if (object->ParticleSystem())
				{
					if (nullptr == object->ParticleSystem()->GetMesh() || nullptr == object->ParticleSystem()->GetMaterial() || nullptr == object->ParticleSystem()->GetMaterial()->GetShader())
					{
						continue;
					}
					Ptr<CGraphicsShader> shader = object->ParticleSystem()->GetMaterial()->GetShader();
					rt = shader->GetRenderTiming();
				}
			
				switch (rt)
				{
				case RENDER_TIMING::FOWARD:
				{
					forward_render_object_vector_.push_back(object);
				}
				break;
				case RENDER_TIMING::PARTICLE:
				{
					particle_object_vector_.push_back(object);
				}
					break;
				case RENDER_TIMING::POST_EFFECT:
				{
					post_effect_vector_.push_back(object);
				}
					break;
				}
			}
		}
	}
}

void CCamera::RenderFoward()
{
	g_transform.view_matrix = view_matrix_;
	g_transform.projection_matrix = projection_matrix_;
	
	for (CGameObject* object : forward_render_object_vector_)
	{
		object->Render();
	}
}

void CCamera::RenderParticle()
{
	for (CGameObject* object : particle_object_vector_)
	{
		object->Render();
	}
}

void CCamera::RenderPostEffect()
{
	for (CGameObject* object : post_effect_vector_)
	{
		CRenderManager::GetInst()->CopyRenderTexture();
		object->Render();
	}
}
