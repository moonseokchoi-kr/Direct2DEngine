#pragma once
#include "CEntity.h"
class CGameObject;
class CComponent :
    public CEntity
{
public:
    CComponent(COMPONENT_TYPE m_type);
    ~CComponent();

public:
    virtual void Update() {}
    virtual void Render() {}

public:
    COMPONENT_TYPE GetType() { return m_type; }
    CGameObject* GetOwner() { return m_owner; }
private:
    COMPONENT_TYPE m_type;
    CGameObject* m_owner;

    friend class CGameObject;
};

