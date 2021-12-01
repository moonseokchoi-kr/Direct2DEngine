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
    Vec3 GetWorldDirection(DIRECTION_TYPE type) { return world_direction_[ENUM_TO_NUMBER(type)]; }
    
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
    array<Vec3, ENUM_TO_NUMBER(DIRECTION_TYPE::END)> local_direction_;
    array<Vec3, ENUM_TO_NUMBER(DIRECTION_TYPE::END)> world_direction_;
    array < Vec3, ENUM_TO_NUMBER(DIRECTION_TYPE::END)> axis_array_ = { Vec3::Right,Vec3::Up,Vec3::Front };

};

