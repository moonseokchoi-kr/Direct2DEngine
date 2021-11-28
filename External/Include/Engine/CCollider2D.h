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
    void SetOffsetPosition(Vec2 position) { offset_position_ = Vec3(position.x, position.y, 0.f); }
    void SetOffsetScale(Vec2 view_scale) { offset_scale_ = Vec3(view_scale.x, view_scale.y, 1.f); }
    Vec3 GetOffsetPosition() { return offset_position_; }
    Vec3 GetOffsetScale() { return offset_scale_; }
    CMesh* GetColliderMesh() { return collider_mesh_.Get(); }
    const Matrix& GetColliderWorldMatrix() { return collider_world_matrix_; }
    int GetCollisionCount() { return collision_count_; }
public:
    virtual void UpdateData();
    virtual void FinalUpdate();

    void Render();

public:
    void OnCollisionEnter(CGameObject* otherObject);
    void OnCollision(CGameObject* otherObject);
    void OnCollisionExit(CGameObject* otherObject);
private:
    Vec3 offset_position_;
    Vec3 final_position_;
    Vec3 offset_scale_;

    Matrix collider_world_matrix_;

    int collision_count_;

    Ptr<CMesh> collider_mesh_;
    Ptr<CMaterial> collider_material_;
};

