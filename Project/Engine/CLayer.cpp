#include "pch.h"
#include "CLayer.h"
#include "CGameObject.h"

CLayer::CLayer()
	:layer_index_(-1)
{
}

CLayer::~CLayer()
{
	Safe_Delete_Vec(parent_object_vector_);
}

void CLayer::Update()
{
	layer_object_vector_.clear();
	for(CGameObject* parent_object : parent_object_vector_)
	{
		parent_object->Update();
	}
}

void CLayer::LateUpdate()
{
	for (CGameObject* parent_object : parent_object_vector_)
	{
		parent_object->LateUpdate();
	}
}

void CLayer::FinalUpdate()
{
	for (CGameObject* parent_object : parent_object_vector_)
	{
		parent_object->FinalUpdate();
	}
}

void CLayer::Render()
{
	for (CGameObject* layer_object : layer_object_vector_)
	{
		layer_object->Render();
	}
}

void CLayer::AddGameObject(CGameObject* object, bool bMove)
{
	parent_object_vector_.push_back(object);

	object->layer_index_ = layer_index_;

	list<CGameObject*> queue;
	queue.push_back(object);

	while (!queue.empty())
	{
		CGameObject* object = queue.front();
		queue.pop_front();
		
		const vector<CGameObject*>& childObjects = object->GetChildObjects();
		for (CGameObject* child : childObjects)
		{
			if (bMove)
			{
				child->layer_index_ = layer_index_;
			}
			else
			{
				if (-1 == child->layer_index_)
					child->layer_index_ = layer_index_;
			}
			queue.push_back(child);
		}
	}
}
