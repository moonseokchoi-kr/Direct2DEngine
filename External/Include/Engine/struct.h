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