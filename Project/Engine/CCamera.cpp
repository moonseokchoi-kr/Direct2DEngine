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
#include "CTileMap.h"

#include "CLayer.h"
#include "CScene.h"

CCamera::CCamera()
	:CComponent(COMPONENT_TYPE::CAMERA)
	, angle_of_view_y_(XM_PI / 2.f)
	, far_z_(10000.f)
	, scale_(1.f)
	, projection_type_(PROJECTION_TYPE::PERSPECTIVE)
	, camera_index_(-1)

{
}

CCamera::~CCamera()
{
}


void CCamera::Update()
{
}

void CCamera::LateUpdate()
{

}

void CCamera::FinalUpdate()
{
	CalViewMatrix();
	CalProjectionMatrix();


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

void CCamera::CalViewMatrix()
{
	// View 행렬 이동파트
	Vec3 vPos = GetTransform()->GetPosition();

	// View 행렬 회전 파트
	Vec3 right = GetTransform()->GetWorldDirection(DIRECTION_TYPE::RIGHT);
	Vec3 up = GetTransform()->GetWorldDirection(DIRECTION_TYPE::UP);
	Vec3 front = GetTransform()->GetWorldDirection(DIRECTION_TYPE::FRONT);

	view_matrix_ = XMMatrixIdentity();

	view_matrix_._11 = right.x;			view_matrix_._12 = up.x;				view_matrix_._13 = front.x;
	view_matrix_._21 = right.y;			view_matrix_._22 = up.y;				view_matrix_._23 = front.y;
	view_matrix_._31 = right.z;			view_matrix_._32 = up.z;				view_matrix_._33 = front.z;
	view_matrix_._41 = -vPos.Dot(right);	view_matrix_._42 = -vPos.Dot(up);		view_matrix_._43 = -vPos.Dot(front);
}

void CCamera::CalProjectionMatrix()
{
	const Vec2 resolution = CDevice::GetInst()->GetResolution();
	if (PROJECTION_TYPE::PERSPECTIVE == projection_type_)
	{
		projection_matrix_ = XMMatrixPerspectiveFovLH(angle_of_view_y_, CDevice::GetInst()->GetAspectRatio(), 1.f, far_z_);
	}
	else if (PROJECTION_TYPE::ORTHO == projection_type_)
	{
		projection_matrix_ = XMMatrixOrthographicLH(resolution.x *  scale_, resolution.y *  scale_, 1.f, far_z_);
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

			for (CGameObject* object : objectVector)
			{
				if (object->MeshRender())
				{
					if (nullptr == object->MeshRender()->GetMesh() 
						|| nullptr == object->MeshRender()->GetCurrentMaterial() 
						|| nullptr == object->MeshRender()->GetCurrentMaterial()->GetShader())
					{
						continue;
					}

					Ptr<CGraphicsShader> shader = object->MeshRender()->GetSharedMaterial()->GetShader();
					rt = shader->GetRenderTiming();
				}
				else if (object->ParticleSystem())
				{
					if (nullptr == object->ParticleSystem()->GetMesh() 
						|| nullptr == object->ParticleSystem()->GetMaterial() 
						|| nullptr == object->ParticleSystem()->GetMaterial()->GetShader())
					{
						continue;
					}
					Ptr<CGraphicsShader> shader = object->ParticleSystem()->GetMaterial()->GetShader();
					rt = shader->GetRenderTiming();
				}
				else if (object->TileMap())
				{
					if (nullptr == object->TileMap()->GetMesh()
						|| nullptr == object->TileMap()->GetMaterial()->GetShader()
						|| nullptr == object->TileMap()->GetMaterial()->GetShader())
					{
						continue;
					}
					Ptr<CGraphicsShader> shader = object->TileMap()->GetMaterial()->GetShader();
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
