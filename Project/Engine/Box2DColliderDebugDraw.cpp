#include "pch.h"
#include "Box2DColliderDebugDraw.h"
#include "CResourceManager.h"
#include "CPhysicsManager.h"
#include "CCore.h"
#include "CDevice.h"
#include "CConstBuffer.h"

Box2DColliderDebugDraw::Box2DColliderDebugDraw()
{
	SetFlags(b2Draw::e_aabbBit);
}

Box2DColliderDebugDraw::~Box2DColliderDebugDraw()
{
}

void Box2DColliderDebugDraw::Init()
{
	collider_material_ = CResourceManager::GetInst()->FindRes<CMaterial>(L"box2d_collider_debug_material");
}

void Box2DColliderDebugDraw::UpdateData()
{
// 	GetGetBoundBox2DBounds();
// 	ScaleCalculate();
// 	CConstBuffer* constantBuffer = CDevice::GetInst()->GetConstBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM);
// 	g_transform.world_matrix = world_matrix;
// 	g_transform.world_view_matrix = g_transform.world_matrix * g_transform.view_matrix;
// 	g_transform.world_view_projection_matrix = g_transform.world_view_matrix * g_transform.projection_matrix;
// 
// 	constantBuffer->SetData(&g_transform, sizeof(Transform));
// 	constantBuffer->SetPipelineStage(PIPELINE_STAGE::PS_VERTEX);
// 	constantBuffer->UpdateData();
}

void Box2DColliderDebugDraw::FinalUpdate()
{

}

void Box2DColliderDebugDraw::Render()
{
	UpdateData();
	collider_material_->UpdateData();
	for (auto& mesh : collider_mesh_)
	{
		mesh.Render();
	}
	Clear();
}

void Box2DColliderDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	vector<VTX> vtx;
	for (UINT i = 0; i < vertexCount; ++i)
	{
		VTX vertex{};
		vertex.color = Vec4(color.r, color.g, color.b, color.a);
		vertex.pos = Vec3(vertices[i].x, vertices[i].y, 100.f);
		vtx.push_back(vertex);
	}
	ColliderRect mesh;
	mesh.Create(vtx.data(), (UINT)vtx.size());
	collider_mesh_.push_back(mesh);
}

void Box2DColliderDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
}

void Box2DColliderDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
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

void Box2DColliderDebugDraw::Clear()
{
	collider_mesh_.clear();
}

void Box2DColliderDebugDraw::GetGetBoundBox2DBounds()
{
	b2World* world = CPhysicsManager::GetInst()->GetPhysicsWorld();
	b2Body* b;
	b2Fixture* fix;
	b2AABB bound;
	float minX, maxX, minY, maxY;

	minX = minY = 1000000.0;
	maxX = maxY = -1000000.0;
	
	b = world->GetBodyList();
	while (b)
	{
		fix = b->GetFixtureList();
		while (fix)
		{
			bound = fix->GetAABB(0);
			if (bound.lowerBound.x < minX)
				minX = bound.lowerBound.x;
			if (bound.upperBound.x > maxX)
				maxX = bound.upperBound.x;
			if (bound.lowerBound.y < minY)
				minY = bound.lowerBound.y;
			if (bound.upperBound.y > maxY)
				maxY = bound.upperBound.y;

			fix = fix->GetNext();
		}

		b = b->GetNext();
	}

	maxX += 2.0;
	maxY += 2.0;
	minX -= 2.0;
	minY -= 2.0;
	b2worldRect.left = (long)minX;
	b2worldRect.right = (long)maxX;
	b2worldRect.bottom = (long)minY;
	b2worldRect.top = (long)maxY;
}

void Box2DColliderDebugDraw::ScaleCalculate()
{
	GetClientRect(CCore::GetInst()->GetMainHwnd(), &windowRect);
	int outputWidth = b2worldRect.right - b2worldRect.left;
	int outputHeight = b2worldRect.bottom - b2worldRect.top;
	int boundsWidth = windowRect.right - windowRect.left;
	int boundsHeight = windowRect.top - windowRect.bottom;

	// ratio of the windows size to the world size
	float scaleX = (float)outputWidth / (float)boundsWidth;
	float scaleY = (float)outputHeight / (float)boundsHeight;
	float scale = scaleX > scaleY ? scaleY : scaleX;


	// move things over if required
	float offsetX = b2worldRect.left - (int)((float)windowRect.left * scaleX);
	float offsetY = b2worldRect.top - (int)((float)windowRect.bottom * scaleY);

	// used to flip the y values
	float yAdjust = b2worldRect.bottom;

	Matrix transformMatrix = XMMatrixTranslation(offsetX, yAdjust-offsetY, 0.f);
	Matrix scaleMatrix = XMMatrixScaling(scaleX, -scaleY, 1.f);
	world_matrix = scaleMatrix*transformMatrix;

}

void ColliderRect::Create(VTX* vtx, UINT vertexCount)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(VTX) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = vtx;

	if (FAILED(DEVICE->CreateBuffer(&desc, &sub, vertex_buffer_.GetAddressOf())))
	{
		assert(nullptr);
	}

	//인덱스버퍼
	vector<UINT> indexBuffer;

	indexBuffer.push_back(0);
	indexBuffer.push_back(1);
	indexBuffer.push_back(2);
	indexBuffer.push_back(2);
	indexBuffer.push_back(3);
	indexBuffer.push_back(0);

	desc.ByteWidth = sizeof(UINT) * indexBuffer.size();
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;

	sub.pSysMem = indexBuffer.data();

	if (FAILED(DEVICE->CreateBuffer(&desc, &sub, index_buffer_.GetAddressOf())))
	{
		assert(nullptr);
	}
}

void ColliderRect::UpdateData()
{
	UINT stride = sizeof(VTX);
	UINT offset = 0;
	if (nullptr != vertex_buffer_)
		CONTEXT->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
	if (nullptr != index_buffer_)
		CONTEXT->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void ColliderRect::Render()
{
	UpdateData();
	CONTEXT->DrawIndexed(6, 0, 0);
}
