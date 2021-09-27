#include "pch.h"
#include "CEntity.h"

UINT CEntity::g_NextId = 0;

CEntity::CEntity()
	:m_ID(++g_NextId)
{
}

CEntity::CEntity(const CEntity& _other)
	:m_ID(++g_NextId)
	,m_name(_other.m_name)
{
}

CEntity::~CEntity()
{
}
