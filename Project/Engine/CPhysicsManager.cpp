#include "pch.h"
#include "CPhysicsManager.h"
#include "CCollisionManager.h"
#include "Box2DColliderDebugDraw.h"

CPhysicsManager::CPhysicsManager()
	:physics_world_(nullptr)
	,gravity_(Vec2(0.0f,0.0f))
{

}

CPhysicsManager::~CPhysicsManager()
{
	SafeDelete(physics_world_);
}

void CPhysicsManager::Init()
{
	physics_world_ = new b2World({ gravity_.x,gravity_.y });
	physics_world_->SetDebugDraw(Box2DColliderDebugDraw::GetInst());
	physics_world_->SetContactListener(CCollisionManager::GetInst());
}

void CPhysicsManager::Update()
{
	const int32_t velocityIterations = 8;
	const int32_t positionIterations = 3;
	physics_world_->Step(fDT, velocityIterations, positionIterations);
}
