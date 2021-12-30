#include "pch.h"
#include "CSceneManager.h"

#include "CScene.h"
#include "CLayer.h"

#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CGameObject.h"



CSceneManager::CSceneManager()
	:current_scene_(nullptr)
	,scene_mode_(SCENE_MODE::STOP)
{
	
}

CSceneManager::~CSceneManager()
{
	SafeDelete(current_scene_);
}


void CSceneManager::Init()
{
	
}

void CSceneManager::Progress()
{
	if (SCENE_MODE::PLAY == scene_mode_)
	{
		current_scene_->Update();
		current_scene_->LateUpdate();
	}
	current_scene_->FinalUpdate();

	if (SCENE_MODE::PLAY == scene_mode_)
	{
		CCollisionManager::GetInst()->Update();
	}
}

CGameObject* CSceneManager::FindObjectByName(const wstring& name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const auto layer = current_scene_->GetLayer(i);

		const auto& object_vector = layer->GetLayerObjects();

		for (const auto object : object_vector)
		{
			if (object->GetName() == name)
			{
				return object;
			}
		}
	}
	return nullptr;
}

void CSceneManager::FindObjectByName(const wstring& name, vector<CGameObject*>& object_vector)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* layer = current_scene_->GetLayer(i);

		const vector<CGameObject*>& objectVector = layer->GetLayerObjects();

		for (const auto object : objectVector)
		{
			if (object->GetName() == name)
			{
				object_vector.push_back(object);
			}
		}
	}
}

void CSceneManager::SetSceneMode(SCENE_MODE mode)
{
	if (SCENE_MODE::STOP == scene_mode_ && SCENE_MODE::PLAY == mode)
		current_scene_->Start();
	scene_mode_ = mode;
}

void CSceneManager::ChangeScene(CScene* nextScene)
{
	SafeDelete(current_scene_);

	current_scene_ = nextScene;
}
