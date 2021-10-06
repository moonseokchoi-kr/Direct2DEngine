#include "pch.h"
#include "CScene.h"

#include "CDevice.h"
#include "CKeyManager.h"
#include "CLayer.h"
CScene::CScene()
{
	for(size_t i = 0; i<layers_.size(); ++i)
	{
		layers_[i] = new CLayer;
	}
}

CScene::~CScene()
{
	Safe_Delete_Array(layers_);
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
	for(size_t i=0; i<layers_.size(); ++i)
	{
		if(nullptr != layers_[i])
		{
			layers_[i]->Update();
		}
	}
}

void CScene::LateUpdate()
{
	for (size_t i = 0; i < layers_.size(); ++i)
	{
		if (nullptr != layers_[i])
		{
			layers_[i]->LateUpdate();
		}
	}
}

void CScene::FinalUpdate()
{
	for (size_t i = 0; i < layers_.size(); ++i)
	{
		if (nullptr != layers_[i])
		{
			layers_[i]->FinalUpdate();
		}
	}
}

void CScene::Render()
{
	CDevice::GetInst()->ClearTarget();

	for (size_t i = 0; i < layers_.size(); ++i)
	{
		if (nullptr != layers_[i])
		{
			layers_[i]->Render();
		}
	}

	CDevice::GetInst()->Present();
}

void CScene::AddGameObject(CGameObject* object, UINT layerType)
{
	assert(layerType < MAX_LAYER);

	layers_[layerType]->AddGameObject(object);
}