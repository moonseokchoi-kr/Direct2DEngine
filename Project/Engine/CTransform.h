#pragma once
#include "CComponent.h"
class CTransform :
    public CComponent
{
public:
    CTransform();
    ~CTransform();

public:
    void FinalUpdate() override;
    virtual void UpdateData();
public:
    Vec3 GetPos() { return pos_; }
    Vec3 GetScale() { return scale_; }
    Vec3 GetRotation() { return rotation_; }

    void SetPos(Vec3 _pos) { pos_ = _pos; }
    void SetScale(Vec3 _scale) { scale_ = _scale; }
    void SetRotation(Vec3 _rotation) { rotation_ = _rotation; }

    Matrix GetWorldMatrix() { return world_matrix_; }
private:
    Matrix world_matrix_;
    Vec3 pos_;
    Vec3 scale_;
    Vec3 rotation_;
};

