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
    Vec3 GetPosition() { return local_position_; }
    Vec3 GetScale() { return local_scale_; }
    Vec3 GetRotation() { return local_rotation_; }

    void SetPosition(Vec3 _pos) { local_position_ = _pos; }
    void SetScale(Vec3 _scale) { local_scale_ = _scale; }
    void SetRotation(Vec3 _rotation) { local_rotation_ = _rotation; }

    Matrix GetWorldMatrix() { return world_matrix_; }
    Vec3 GetWorldPos() { return world_matrix_.Translation(); }
    Vec3 GetWorldScale();
public:
    CLONE(CTransform);
private:
    Matrix world_matrix_;
 
    Vec3 local_position_;
    Vec3 local_scale_;
    Vec3 local_rotation_;
};

