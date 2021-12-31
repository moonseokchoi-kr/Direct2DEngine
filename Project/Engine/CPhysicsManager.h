#pragma once
#include "singleton.h"

class b2World;

class CPhysicsManager :
    public CSingleton<CPhysicsManager>
{
    SINGLE(CPhysicsManager);

public:
    void Init();
    void Update();
public:
    void SetGravity(Vec2 gravity) { gravity_ = gravity; }
    void DebugDraw() { physics_world_->DebugDraw(); }
    b2World* GetPhysicsWorld() { return physics_world_; }
private:
    Vec2 gravity_;
    b2World* physics_world_;
};



