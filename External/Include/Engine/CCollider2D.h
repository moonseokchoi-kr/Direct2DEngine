#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CCollider2D :
    public CComponent
{
public:
    CCollider2D();
    ~CCollider2D();

public:
    CLONE(CCollider2D);

public:
    void SetOffsetPosition(Vec2 position) { offset_position_ = Vec3(position.x,position.y, 0.f); }
    void SetOffsetScale(Vec2 scale) { offset_scale_ = Vec3(scale.x, scale.y, 1.f); }
    const Matrix& GetColliderWorldMatrix() { return collider_world_matrix_; }

public:
    virtual void UpdateData();
    virtual void FinalUpdate();

    void Render();

public:
    void OnCollisionEnter(CCollider2D* otherCollider);
    void OnCollision(CCollider2D* otherCollider);
    void OnCollisionExit(CCollider2D* otherCollider);
private:
    Vec3 offset_position_;
    Vec3 final_position_;
    Vec3 offset_scale_;

    Matrix collider_world_matrix_;

    int collision_count_;

    Ptr<CMesh> collider_mesh_;
    Ptr<CMaterial> collider_material_;
};
