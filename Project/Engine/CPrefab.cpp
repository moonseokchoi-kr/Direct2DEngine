#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"
CPrefab::CPrefab()
    :prototype_object_(nullptr)
{
}

CPrefab::CPrefab(CGameObject* gameObject)
    :prototype_object_(gameObject)
{
}

CPrefab::~CPrefab()
{
    SafeDelete(prototype_object_);
}

CGameObject* CPrefab::Instantiate()
{
    return prototype_object_->Clone();
}
