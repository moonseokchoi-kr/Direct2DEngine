#include "pch.h"
#include "CRigidBody2D.h"

#include "CTimeManager.h"

#include "CGameObject.h"
#include "CTransform.h"

CRigidBody2D::CRigidBody2D()
	:CComponent(COMPONENT_TYPE::RIGIDBODY2D)
	, mass_(1.f)
	, max_velocity_(400.f)
	, static_friction_(100.f)
	, is_active_(true)
	, density_(1.f)
	, linear_damping_(0.1f)
	, angular_damping_(0.1f)
	, angular_velocity_(100.f)
{
}

CRigidBody2D::~CRigidBody2D()
{
}

void CRigidBody2D::FinalUpdate()
{
	CalCulateMass();
	if (!is_active_ || mass_ == 0.0f)
		return;
	if (0!=force_.Length())
	{
		accel_ = force_ * inverse_mass_;
	}
	if (0 != torque_)
	{
		angular_velocity_ += torque_ * inverse_intertia_ * fDT;
	}
	accel_ += additional_accel_;

	velocity_ += accel_ * fDT;
	if (0 != velocity_.Length())
	{
		Vec2 fricDir = -velocity_;
		fricDir.Normalize();
		Vec2 friction = fricDir * static_friction_*fDT;
		if (velocity_.Length() <= friction.Length())
		{
			velocity_ = Vec2();
		}
		else
		{
			velocity_ += friction;
		}
	}
	if (max_velocity_ < velocity_.Length())
	{
		velocity_.Normalize();
		velocity_ *= max_velocity_;
	}
	
	Move();

	force_ = Vec2();
	accel_ = Vec2();
	additional_accel_ = Vec2();
}

void CRigidBody2D::ApplyImpulse(const Vec2& impulse)
{
	velocity_ += inverse_mass_ * impulse;
}

void CRigidBody2D::ApplyImpulse(const Vec2& impulse, const Vec2& contactVector)
{
	ApplyImpulse(impulse);
	angular_velocity_ += inverse_intertia_ * contactVector.Cross(impulse);
}

void CRigidBody2D::SetStatic()
{
	intertia_ = 0.f;
	inverse_intertia_ = 0.f;
	mass_ = 0.f;
	inverse_mass_ = 0.f;
}

bool CRigidBody2D::IsStatic()
{
	return intertia_ ==0.f && mass_ == 0.f;
}

void CRigidBody2D::ResolveCollide(CGameObject* other)
{
	Vec2 otherVlc = other->RigidBody2D()->GetVelocity();
	
	Vec2 rv = velocity_ - otherVlc;
	Vec2 normal = Vec2(1.f, 1.f);
	normal.Normalize();
	float velAlongNormal = rv.Dot(normal);

	if (velAlongNormal > 0)
		return;

	float e = min(other->RigidBody2D()->GetRestitution(), restitution_);

	float j = -(1 + e) * velAlongNormal;
	j /= inverse_mass_ + 1/other->RigidBody2D()->GetMass();

	//ApplyImpulse
	ApplyImpulse(-j * normal);
	other->RigidBody2D()->ApplyImpulse(j * normal);
}

void CRigidBody2D::SaveToScene(FILE* file)
{
	CComponent::SaveToScene(file);

	fwrite(&force_, sizeof(Vec2), 1, file);
	fwrite(&velocity_, sizeof(Vec2), 1, file);
	fwrite(&accel_, sizeof(Vec2), 1, file);
	fwrite(&static_friction_, sizeof(Vec2), 1, file);
	fwrite(&additional_accel_, sizeof(Vec2), 1, file);

	fwrite(&mass_, sizeof(float), 1, file);
	fwrite(&max_velocity_, sizeof(float), 1, file);
	fwrite(&static_friction_, sizeof(float), 1, file);
	fwrite(&is_active_, sizeof(bool), 1, file);

}

void CRigidBody2D::LoadFromScene(FILE* file)
{
	CComponent::LoadFromScene(file);

	fread(&force_, sizeof(Vec2), 1, file);
	fread(&velocity_, sizeof(Vec2), 1, file);
	fread(&accel_, sizeof(Vec2), 1, file);
	fread(&static_friction_, sizeof(Vec2), 1, file);
	fread(&additional_accel_, sizeof(Vec2), 1, file);

	fread(&mass_, sizeof(float), 1, file);
	fread(&max_velocity_, sizeof(float), 1, file);
	fread(&static_friction_, sizeof(float), 1, file);
	fread(&is_active_, sizeof(bool), 1, file);
}

void CRigidBody2D::Move()
{
	Vec3 pos = GetOwner()->Transform()->GetPosition();
	pos += Vec3(velocity_.x,velocity_.y,0) * fDT;
	GetOwner()->Transform()->SetPosition(pos);
}

void CRigidBody2D::CalCulateMass()
{
	inverse_mass_ = mass_ ? 1.f / mass_ : 0.f;

	//calculate intertia

}
