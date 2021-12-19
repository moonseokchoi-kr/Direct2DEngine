#pragma once

struct Vertex
{
	Vec3 pos;
	Vec4 color;
	Vec2 uv;
};

typedef Vertex VTX;

struct AnimationData
{
	Vec2 left_top;
	Vec2 size;
	Vec2 full_size;
	Vec2 offset;
	UINT using_anim;
	UINT padding[3];
};

struct AnimationFrame
{
	AnimationData animation_data;
	float duration;
};

struct LightColor
{
	Vec4 light_color;
	Vec4 light_specular;
	Vec4 light_ambient;
};

struct LightInfo
{
	LightColor light_color;
	LIGHT_TYPE light_type;
	Vec3 light_direction;
	Vec3 light_position;

	float light_range;
	float light_angle;
	Vec3 padding;
};


struct Transform
{
	Matrix world_matrix;
	Matrix view_matrix;
	Matrix projection_matrix;
	Matrix world_view_matrix;
	Matrix world_view_projection_matrix;
};

struct MaterialParameter
{
	array<int, 4> int_arr;
	array<float, 4> float_arr;
	array<Vec2, 4> vec2_arr;
	array<Vec4, 4> vec4_arr;
	array<Matrix, 4> matrix_arr;
};


struct Particle
{
	Vec3 world_position;
	Vec3 view_scale;
	Vec3 view_rotation;
	Vec3 color;
	Vec3 direction;
	float current_time;
	float max_life_time;
	int is_active;
};


struct ParticleShadredData
{
	int activable_count;
	int current_active_count;
	Vec3 range;
};

struct GlobalData
{
	Vec2 resolution;
	Vec2 noise_texture_resolution;
	float delta_time;
	float accumulated_time;
	Vec2 padding;
};

struct Tile
{
	int index;
	bool moveable = true;
};