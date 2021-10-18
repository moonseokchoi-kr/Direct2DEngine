#include "pch.h"
#include "CScript.h"
#include "CEventManager.h"

CScript::CScript()
	:CComponent(COMPONENT_TYPE::SCRIPT)
	,script_type_(-1)
{
}

CScript::~CScript()
{
}

void CScript::CreateObject(CGameObject* object, int layerIndex)
{
	Event evn = {};

	evn.event_type = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)object;
	evn.wParam = (DWORD_PTR)layerIndex;

	CEventManager::GetInst()->AddEvent(evn);

}

void CScript::CreateObject(CGameObject* object, Vec3 position, int layerIndex)
{
	Event evn = {};
	object->Transform()->SetPos(position);
	evn.event_type = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)object;
	evn.wParam = (DWORD_PTR)layerIndex;

	CEventManager::GetInst()->AddEvent(evn);
}

void CScript::DeleteObject(CGameObject* object, float delay)
{
	Event evn = {};

	evn.event_type = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)object;
	evn.wParam = (DWORD_PTR)delay;

	CEventManager::GetInst()->AddEvent(evn);
}
