#include "pch.h"
#include "CLayer.h"
#include "CGameObject.h"

CLayer::CLayer()
{
}

CLayer::~CLayer()
{
	Safe_Delete_Vec(objects_);
}

void CLayer::Update()
{
	for(size_t i=0; i<objects_.size(); ++i)
	{
		objects_[i]->Update();
	}
}

void CLayer::LateUpdate()
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		objects_[i]->LateUpdate();
	}
}

void CLayer::FinalUpdate()
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		objects_[i]->FinalUpdate();
	}
}

void CLayer::Render()
{
	for (size_t i = 0; i < objects_.size(); ++i)
	{
		objects_[i]->Render();
	}
}

void CLayer::AddGameObject(CGameObject* object)
{
	objects_.push_back(object);
}
