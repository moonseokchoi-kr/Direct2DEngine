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
    Vec3 GetPos() { return pos_; }
    Vec3 GetScale() { return scale_; }
    Vec3 GetRotation() { return rotation_; }

    void SetPos(Vec3 _pos) { pos_ = _pos; }
    void SetScale(Vec3 _scale) { scale_ = _scale; }
    void SetRotation(Vec3 _rotation) { rotation_ = _rotation; }
private:
    Matrix world_matrix_;
    __declspec(property(get = GetPos, put = SetPos)) Vec3 pos_;
    __declspec(property(get = GetScale, put = SetScale)) Vec3 scale_;
    __declspec(property(get = GetRotation, put = SetRotation)) Vec3 rotation_;
};

