#include "pch.h"
#include "CCamera.h"


#include "CTransform.h"

#include "CDevice.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CSceneManager.h"
#include "CRenderManager.h"

#include "CLayer.h"
#include "CScene.h"

CCamera::CCamera()
	:CComponent(COMPONENT_TYPE::CAMERA)
	, angle_of_view_y_(90.f)
	, far_z_(10000.f)
	, projection_type_(PROJECTION_TYPE::ORTHO)
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
		projection_matrix_ = XMMatrixPerspectiveFovLH(angle_of_view_y_, CDevice::GetInst()->GetAspectRatio(), 1, far_z_);
	}
	else if(PROJECTION_TYPE::ORTHO == projection_type_)
	{
		projection_matrix_ = XMMatrixOrthographicLH(resolution.x, resolution.y, 1, far_z_);
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
