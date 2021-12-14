#include "pch.h"
#include "CEventManager.h"
#include "CSceneManager.h"
#include "CScene.h"
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
	//이전프레임의 삭제할 오브젝트 삭제
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

	}
	break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		//lParam : Object
		//wParam : None
		//Object를 Dead상태로 변경
		//삭제예정오브젝트들을 모아둔다.
		CGameObject* object = (CGameObject*)_event.lParam;
		if (OBJECT_STATE::ALIVE == object->GetObjectState())
		{
 			object->SetDead(static_cast<float>(_event.wParam));
			dead_object_vector_.push_back(object);
		}
	}
	break;

	default:
		break;
	}
}