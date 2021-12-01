#include "pch.h"
#include "ToolGameObject.h"

#include <Engine/CComponent.h>
#include <Engine/CScript.h>

ToolGameObject::ToolGameObject()
{
}

ToolGameObject::~ToolGameObject()
{
}

void ToolGameObject::Start()
{

}

void ToolGameObject::FinalUpdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* comp = GetComponent((COMPONENT_TYPE)i);
		if (nullptr != comp)
		{
			comp->FinalUpdate();
		}
	}

	const vector<CScript*>& scripts = GetScripts();

	for (const auto& script : scripts)
	{
		script->FinalUpdate();
	}

	const vector<CGameObject*> vecChild = GetChildObjects();

	for (const auto& child : vecChild)
	{
		child->FinalUpdate();
	}
}
