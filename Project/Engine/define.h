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
	PS_COMPUTE = 0x20,

	PS_NONPIXEL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY| PS_COMPUTE,
	PS_ALL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL|PS_COMPUTE,

};


enum class CONSTANT_BUFFER_TYPE
{
	TRANSFORM,
	MATERIAL_CONST,
	GLOBAL,
	LIGHT2D,
	ANIMATE2D,
	END,
};


enum class RESOURCE_TYPE
{
	MESH,
	GRAPHIC_SHADER,
	COMPUTE_SHADER,
	TEXTURE,
	MATERIAL,
	METADATA,
	PREFAB,
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
	RIGIDBODY2D,
	RIGIDBODY3D,
	TERRAIN,
	TILEMAP,
	SKYBOX,
	SOUND,
	UI,
	END,

	SCRIPT,
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
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,
	TEX_ARR_0,
	TEX_ARR_1,
	TEX_CUBE_0,
	TEX_CUBE_1,
	TEX_END,

	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	ADD_CHILD,
	DELETE_OBJECT,
	DELETE_DELAY_OBJECT,
	SCENE_CHANGE,
};


enum class BLEND_TYPE
{
	DEFAULT,
	ALPHA_BLEND,
	ALPHA_BLEND_COVERAGE,
	ONE_ONE,

	END,
};

enum class RASTERIZER_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	CULL_WIREFRAME,

	END
};


enum class DEPTH_STENCIL_TYPE
{
	LESS,
	LESS_EQUAL,
	GRATER,
	GRATER_EQUAL,
	NO_TEST,
	NO_WRITE,
	NO_TEST_NO_WRITE,

	END,

};

enum class LIGHT_TYPE
{
	DIR,
	POINT,
	SPOT,
};

enum class RENDER_TIMING
{
	FOWARD,
	PARTICLE,
	POST_EFFECT,
	NONE,
};

enum class SCENE_MODE
{
	PLAY,
	PAUSE,
	STOP,
};

enum class DIRECTION_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END,
};

enum class UI_TYPE
{
	CANVAS,
	LIST,
	GRID,
	//새 클래스를 생성할때마다 추가 
	END,
};