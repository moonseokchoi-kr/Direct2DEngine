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

