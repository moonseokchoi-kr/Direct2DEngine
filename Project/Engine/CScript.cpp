#include "pch.h"
#include "CScript.h"
#include "CEventManager.h"

CScript::CScript()
	:CComponent(COMPONENT_TYPE::SCRIPT)
	,script_type_(-1)
{
}

CScript::CScript(int scriptType)
	:CComponent(COMPONENT_TYPE::SCRIPT)
	,script_type_(scriptType)
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

void CScript::DeleteObject(CGameObject* object, float delay)
{
	Event evn = {};

	evn.event_type = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)object;
	evn.wParam = (DWORD_PTR)delay;

	CEventManager::GetInst()->AddEvent(evn);
}

void CScript::Instantiate(Ptr<CPrefab> prefab, int layerIndex)
{
	CGameObject* prototype = prefab->Instantiate();
	CreateObject(prototype, layerIndex);
}

void CScript::Instantiate(Ptr<CPrefab> prefab, Vec3 position, int layerIndex,Vec3 rotation)
{
	CGameObject* prototype = prefab->Instantiate();
	prototype->Transform()->SetPosition(position);
	prototype->Transform()->SetRotation(rotation);
	CreateObject(prototype, layerIndex);
}

void CScript::OnCollisionEnter(CGameObject* otherObject)
{
}

void CScript::OnCollision(CGameObject* otherObject)
{
}

void CScript::OnCollisionExit(CGameObject* otherObject)
{
}
