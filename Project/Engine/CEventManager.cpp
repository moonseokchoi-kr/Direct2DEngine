#include "pch.h"
#include "CEventManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"

CEventManager::CEventManager()
{

}

CEventManager::~CEventManager()
{
	Safe_Delete_Vec(dead_object_vector_);
}


void CEventManager::Update()
{
	occured_object_event_ = false;
	//������������ ������ ������Ʈ ����
	for (size_t i = 0; i < dead_object_vector_.size(); ++i)
	{
		if (nullptr!=dead_object_vector_[i]&&dead_object_vector_[i]->IsDead())
		{
			SafeDelete(dead_object_vector_[i]);
			occured_object_event_ = true;
		}
			
	}

	for (size_t i = 0; i < event_vector_.size(); ++i)
	{
		excute(event_vector_[i]);
	}
	event_vector_.clear();
}

void CEventManager::excute(const Event& _event)
{
	switch (_event.event_type)
	{

	case EVENT_TYPE::CREATE_OBJECT:
	{
		//lParam : Object
		//wParam : GROUPTYPE
		CGameObject* object = (CGameObject*)_event.lParam;

		CSceneManager::GetInst()->GetCurrentScene()->AddGameObject(object, static_cast<UINT>(_event.wParam),true);

		occured_object_event_ = true;
	}

	break;
	case EVENT_TYPE::ADD_CHILD:
	{
		//lParam : Parent Object
		//wParam : Child Object
		CGameObject* parent = reinterpret_cast<CGameObject*>(_event.lParam);
		CGameObject* child = reinterpret_cast<CGameObject*>(_event.wParam);

		if (child->GetParent())
		{
			child->SeparateFromParent();
		}
		else
		{
			if (-1 != child->GetLayerIndex())
			{
				CScene* currentScene = CSceneManager::GetInst()->GetCurrentScene();
				CLayer* layer = currentScene->GetLayer(child->GetLayerIndex());
				layer->DeregisterAsParentObject(child);
			}
		}
		parent->AddChild(child);
		occured_object_event_ = true;
	}
	break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		//lParam : Object
		//wParam : None
		//Object�� Dead���·� ����
		//��������������Ʈ���� ��Ƶд�.
		CGameObject* object = (CGameObject*)_event.lParam;
		if (OBJECT_STATE::ALIVE == object->GetObjectState())
		{
 			object->SetDead(static_cast<float>(_event.wParam));
			dead_object_vector_.push_back(object);
		}
	}
	break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		//lparam : ������ Scene
		CSceneManager::GetInst()->ChangeScene((CScene*)_event.lParam);
		occured_object_event_ = true;
	}
		break;
	default:
		break;
	}
}