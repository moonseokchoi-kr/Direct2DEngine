#include "pch.h"
#include "ToolObjectManager.h"
#include "ToolGameObject.h"
#include "ToolCamera.h"

#include <Engine/CTransform.h>
#include <Engine/CRenderManager.h>
#include <Engine/CSceneManager.h>
ToolObjectManager::ToolObjectManager()
{

}

ToolObjectManager::~ToolObjectManager()
{
	Safe_Delete_Vec(tool_object_vector_);
}

void ToolObjectManager::Init()
{
	ToolGameObject* camera = nullptr;

	camera = new ToolGameObject;
	camera->SetName(L"tool_camera");
	camera->AddComponent(new CTransform);
	camera->AddComponent(new ToolCamera);

	camera->Camera()->SetProjectionType(PROJECTION_TYPE::PERSPECTIVE);
	camera->Camera()->CheckAllLayout();

	CRenderManager::GetInst()->RegisterToolCamera(camera->Camera());

	tool_object_vector_.push_back(camera);
}

void ToolObjectManager::Update()
{
	SCENE_MODE mode = CSceneManager::GetInst()->GetSceneMode();

	if (SCENE_MODE::PLAY != mode)
	{
		for (const auto& object : tool_object_vector_)
		{
			object->Update();
		}
		for (const auto& object : tool_object_vector_)
		{
			object->LateUpdate();
		}
		for (const auto& object : tool_object_vector_)
		{
			object->FinalUpdate();
		}
	}
}

void ToolObjectManager::Render()
{
}
