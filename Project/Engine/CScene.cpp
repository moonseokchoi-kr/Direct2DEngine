#include "pch.h"
#include "CScene.h"

#include "CDevice.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CLayer.h"
CScene::CScene()
{
	int count = 0;
	//절대 0번 레이어는 사용하지 말것
	for(UINT i=0; i<layer_array_.size(); ++i)
	{
		layer_array_[i] = new CLayer;
		layer_array_[i]->layer_index_ = i;
	}
}

CScene::~CScene()
{
	Safe_Delete_Array(layer_array_);
}

void CScene::Start()
{
	for (const auto& layer : layer_array_)
	{
		if (nullptr != layer)
		{
			layer->Start();
		}
	}
}

void CScene::Update()
{

	if (KEY_HOLD(KEY::ALT))
	{
		if (KEY_HOLD(KEY::ENTER))
		{
			if (CDevice::GetInst()->IsWindow())
				CDevice::GetInst()->OnReSize(Vec2(1920, 1080));
			else
				CDevice::GetInst()->OnReSize(Vec2(1600, 900));
		}
	}
	for(const auto& layer : layer_array_)
	{
		if(nullptr != layer)
		{
			layer->Update();
		}
	}
}

void CScene::LateUpdate()
{
	//physics update
	for (const auto& layer : layer_array_)
	{
		if (nullptr != layer)
		{
			layer->LateUpdate();
		}
	}
}

void CScene::FinalUpdate()
{
	for (const auto& layer : layer_array_)
	{
		if (nullptr != layer)
		{
			layer->layer_object_vector_.clear();
		}
	}
	for (const auto& layer : layer_array_)
	{
		if (nullptr != layer)
		{
			layer->FinalUpdate();
		}
	}
}

void CScene::Render()
{
// 	for (const auto& layer : layer_array_)
// 	{
// 		if (nullptr != layer)
// 		{
// 			layer->Render();
// 		}
// 	}
}

void CScene::AddGameObject(CGameObject* object, UINT layerType, bool bMove)
{
	assert(layerType < MAX_LAYER);

	layer_array_[layerType]->AddGameObject(object, bMove);
}

CGameObject* CScene::GetObjectWithName(const wstring& objectName)
{
	CGameObject* object = nullptr;
	for (const auto& layer : layer_array_)
	{
		object = layer->FindObjectWithName(objectName);
		if (nullptr != object)
			return object;
	}
	return object;
}
