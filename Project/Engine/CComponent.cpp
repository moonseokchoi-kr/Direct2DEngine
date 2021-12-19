#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _type)
	:type_(_type)
	,owner_(nullptr)
{
}

CComponent::~CComponent()
{
}

void CComponent::SaveToScene(FILE* file)
{
	UINT type = (UINT)type_;
	fwrite(&type, sizeof(UINT), 1, file);
}

void CComponent::LoadFromScene(FILE* file)
{
}

