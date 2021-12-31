#include "pch.h"
#include "Box2DColliderDebugDraw.h"

Box2DColliderDebugDraw::Box2DColliderDebugDraw()
{
	SetFlags(b2Draw::e_aabbBit);
	
}

Box2DColliderDebugDraw::~Box2DColliderDebugDraw()
{
	Safe_Delete_Vec(collider_mesh_vector_);
}

void Box2DColliderDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	vector<VTX> vertexBuffer;
	for (int i = 0; i < vertexCount; ++i)
	{
		VTX vertex{};
		vertex.pos = Vec3(vertices[i].x, vertices[i].y, 100.f);
		vertex.color = Vec4(color.r, color.g, color.b, color.a);
		vertexBuffer.push_back(vertex);
	}
	vector<UINT> indexBuffer;

	indexBuffer.push_back(0);
	indexBuffer.push_back(1);
	indexBuffer.push_back(2);
	indexBuffer.push_back(2);
	indexBuffer.push_back(3);
	indexBuffer.push_back(0);
	CMesh* mesh = new CMesh;
	mesh->Create(vertexBuffer.data(), (UINT)vertexBuffer.size(), indexBuffer.data(), (UINT)indexBuffer.size());
	collider_mesh_vector_.push_back(mesh);
}

void Box2DColliderDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

	vector<VTX> vertexBuffer;
	for (int i = 0; i < vertexCount; ++i)
	{
		VTX vertex{};
		vertex.pos = Vec3(vertices[i].x, vertices[i].y, 100.f);
		vertex.color = Vec4(color.r, color.g, color.b, color.a);
		vertexBuffer.push_back(vertex);
	};
}

void Box2DColliderDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	VTX vertex{};
	vertex.pos = Vec3(center.x,center.y,100.f);
	vertex.uv = Vec2(0.5f, 0.5f);
	vertex.color = Vec4(color.r,color.g,color.b,color.a);
	
	vector<VTX> vertexBuffer;
	vertexBuffer.push_back(vertex);

	//원의 테두리 정점
	UINT sliceCount = 80;
	float angle = XM_2PI / (float)sliceCount;

	float theta = 0;
	for (UINT i = 0; i < sliceCount + 1; ++i)
	{
		vertex.pos = Vec3(radius * cosf(theta), radius * sinf(theta), 0.f);
		vertex.uv = Vec2(vertex.pos.x / (2.f * radius) + 0.5f, 0.5f - vertex.pos.y / (2.f * radius));
		vertex.color = Vec4(1.0f, 0.f, 1.0f, 1.0f);

		vertexBuffer.push_back(vertex);

		theta += angle;
	}
}

void Box2DColliderDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
}

void Box2DColliderDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
}

void Box2DColliderDebugDraw::DrawTransform(const b2Transform& xf)
{
}

void Box2DColliderDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
}

void Box2DColliderDebugDraw::UpdateData()
{
	collider_material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"box2d_collider_debug_material");
	collider_material_->UpdateData();
	for (CMesh* mesh : collider_mesh_vector_)
	{
		mesh->UpdateData();
	}
}

void Box2DColliderDebugDraw::FinalUpdate()
{

}

void Box2DColliderDebugDraw::Render()
{
	UpdateData();
	for (CMesh* mesh : collider_mesh_vector_)
	{
		mesh->Render();
	}
	collider_mesh_vector_.clear();
}
