#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _type)
	:m_type(_type)
	,m_owner(nullptr)
{
}

CComponent::~CComponent()
{
}

