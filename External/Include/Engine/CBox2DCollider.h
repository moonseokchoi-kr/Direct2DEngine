#pragma once
#include "CComponent.h"
#include "Box2DColliderDebugDraw.h"
class CBox2DCollider :
    public CComponent
{
public:
    CBox2DCollider();
    CBox2DCollider(const CBox2DCollider& origin);
    ~CBox2DCollider();
public:
    void Start() override;
    void FinalUpdate() override;
    void LateUpdate() override;
    void Render() override;
    CLONE(CBox2DCollider);
public:
    Vec2 GetOffset() { return offset_; }
    Vec2 GetOffsetSize() { return offset_size_; }
    float GetDenisty() { return denisty_;}
    float GetFriction() { return friction_; }
    float GetRestitution() { return restitution_; }
    float GetRestitutionThreshold() { return restitution_threshold_; }

	void SetOffset(Vec2 offset) {  offset_ = offset; }
	void SetOffsetSize(Vec2 offsetSize) {  offset_size_ = offsetSize; }
	void SetDenisty(float denisty) {  denisty_ = denisty; }
	void SetFriction(float friction) {  friction_ = friction; }
	void SetRestitution(float restitution) {  restitution_ = restitution; }
	void SetRestitutionThreshold(float restitution_threshold) {  restitution_threshold_ = restitution_threshold; }

private:
    void InitCollider();
private:
    Vec2 offset_;
    Vec2 offset_size_;

    void* runtime_fixture_;
    // TODO(Yan)
    float denisty_;
    float friction_;
    float restitution_;
    float restitution_threshold_;

    Box2DColliderDebugDraw debug_draw_;
};
