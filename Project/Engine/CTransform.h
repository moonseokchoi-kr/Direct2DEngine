#pragma once
#include "CComponent.h"
class CTransform :
    public CComponent
{
public:
    CTransform();
    ~CTransform();

public:
    virtual void Update();
    virtual void UpdateData();
public:
    Vec3 GetPos() { return m_pos; }
    Vec3 GetScale() { return m_scale; }
    Vec3 GetRotation() { return m_rotation; }

    void SetPos(Vec3 _pos) { m_pos = _pos; }
    void SetScale(Vec3 _scale) { m_scale = _scale; }
    void SetRotation(Vec3 _rotation) { m_rotation = _rotation; }
private:
    Vec3 m_pos;
    Vec3 m_scale;
    Vec3 m_rotation;

    __declspec(property(get = GetPos, put = SetPos)) Vec3 m_pos;
    __declspec(property(get = GetScale, put = SetScale)) Vec3 m_scale;
    __declspec(property(get = GetRotation, put = SetRotation)) Vec3 m_rotation;
};

