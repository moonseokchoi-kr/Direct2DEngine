#pragma once

#define SINGLE(type) private:\
						type();\
						~type();\
						friend class CSingleton<type>;

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;
