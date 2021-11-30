#pragma once
#include <Engine/CGameObject.h>
class ToolGameObject :
    public CGameObject
{
public:
    ToolGameObject();
    ~ToolGameObject();
public:
	void FinalUpdate();
};

