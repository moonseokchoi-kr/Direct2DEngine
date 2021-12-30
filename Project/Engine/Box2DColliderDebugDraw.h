#pragma once
#include "CMaterial.h"
#include "CMesh.h"
#include <Box2d/b2_draw.h>

class CMesh;


class Box2DColliderDebugDraw :
    public b2Draw
{
public:
    Box2DColliderDebugDraw();
    ~Box2DColliderDebugDraw();


    // b2Draw을(를) 통해 상속됨
    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

    virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

    virtual void DrawTransform(const b2Transform& xf) override;

    virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;
public:
    void UpdateData();
    void FinalUpdate();
    void Render();
private:
	vector<Ptr<CMesh>> collider_mesh_vector_;
	Ptr<CMaterial> collider_material_;
};

