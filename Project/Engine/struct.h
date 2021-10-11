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
};

struct MaterialParameter
{
	array<int, 4> int_arr;
	array<float, 4> float_arr;
	array<Vec2, 4> vec2_arr;
	array<Vec4, 4> vec4_arr;
	array<Matrix, 4> matrix_arr;
};