#include "pch.h"
#include "CEntity.h"

UINT CEntity::next_id_ = 0;

CEntity::CEntity()
	:id_(++next_id_)
{
}

CEntity::CEntity(const CEntity& _other)
	:id_(++next_id_)
	,name_(_other.name_)
{
}

CEntity::~CEntity()
{
}
