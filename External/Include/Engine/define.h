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

#define ENUM_TO_NUMBER(t) static_cast<UINT>(t)

#define GET_COMPONENT(ComponentName, EnumName) C##ComponentName* ComponentName(){return (C##ComponentName*)component_array_[(UINT)(EnumName)];}

#define MAX_LAYER 32
typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;


enum PIPELINE_STAGE
{
	PS_VERTEX = 0x01,
	PS_HULL = 0x02,
	PS_DOMAIN = 0x04,
	PS_GEOMETRY = 0x08,
	PS_PIXEL = 0x10,

	PS_NONPXL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY,
	PS_ALL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL,

};


enum class CB_TYPE
{
	TRANSFORM,
	MATERIAL_CONST,
	END,
};


enum class RES_TYPE
{
	MESH,
	SHADER,
	TEXTURE,
	MATERIAL,
	METADATA,
	SOUND,
	END,
};

enum class COMPONENT_TYPE
{
	TRANSFORM,
	MESHRENDER,
	COLLIDER2D,
	COLLIDER3D,
	CAMERA,
	LIGHT2D,
	LIGHT3D,
	PARTICLESYSTEM,
	ANIMATOR2D,
	ANIMATOR3D,
	TERRAIN,
	SKYBOX,
	SCRIPT,
	SOUND,
	END,
};

enum class PROJECTION_TYPE
{
	PERSPECTIVE,
	ORTHO,
	END
};

enum class SHADER_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,

	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_ARR_0,
	TEX_ARR_1,
	TEX_CUBE_0,
	TEX_CUBE_1,
	TEX_END,

	END,
};