#pragma once

struct Vertex
{
	Vec3 pos;
	Vec4 color;
	Vec2 uv;
};

typedef Vertex VTX;

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

