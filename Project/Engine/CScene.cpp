#include "pch.h"
#include "CScene.h"

#include "CDevice.h"
#include "CKeyManager.h"
#include "CLayer.h"
CScene::CScene()
{
	int count = 0;
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
	for(size_t i=0; i<layer_array_.size(); ++i)
	{
		if(nullptr != layer_array_[i])
		{
			layer_array_[i]->Update();
		}
	}
}

void CScene::LateUpdate()
{
	for (size_t i = 0; i < layer_array_.size(); ++i)
	{
		if (nullptr != layer_array_[i])
		{
			layer_array_[i]->LateUpdate();
		}
	}
}

void CScene::FinalUpdate()
{
	for (size_t i = 0; i < layer_array_.size(); ++i)
	{
		if (nullptr != layer_array_[i])
		{
			layer_array_[i]->FinalUpdate();
		}
	}
}

void CScene::Render()
{

	for (size_t i = 0; i < layer_array_.size(); ++i)
	{
		if (nullptr != layer_array_[i])
		{
			layer_array_[i]->Render();
		}
	}
}

void CScene::AddGameObject(CGameObject* object, UINT layerType, bool bMove)
{
	assert(layerType < MAX_LAYER);

	layer_array_[layerType]->AddGameObject(object, bMove);
}

CGameObject* CScene::GetObjectWithName(const wstring& objectName)
{
	CGameObject* object = nullptr;
	for (CLayer* layer : layer_array_)
	{
		object = layer->FindObjectWithName(objectName);
		if (nullptr != object)
			return object;
	}
	return object;
}
