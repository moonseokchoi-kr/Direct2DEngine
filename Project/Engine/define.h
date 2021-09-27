#pragma once

#define SINGLE(type) private:\
						type();\
						~type();\
						friend class CSingleton<type>;

#define KEY_CHECK(key, state) CKeyManager::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define MOUSE_POS CKeyManager::GetInst()->GetMousePos()

#define fDT CTimeManager::GetInst()->GetfDT()
#define DT CTimeManager::GetInst()->GetDT()

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define CLONE(type) type* Clone() {return new type(*this);}

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;
