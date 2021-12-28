#pragma once
#include "CEntity.h"


enum class OBJECT_STATE
{
    ALIVE,
    RESERVE_DEAD,
    DEAD,
};


class CComponent;
class CMeshRender;
class CTransform;
class CCollider2D;
class CCamera;
class CScript;
class CLight2D;
class CAnimator2D;
class CParticleSystem;
class CTileMap;
class CRigidBody2D;
class CUIComponent;

class CGameObject :
    public CEntity
{
public:
    CGameObject();
    CGameObject(const CGameObject& origin);
    virtual ~CGameObject();

public:
    void Start();
    void Update();
    void LateUpdate();
    void FinalUpdate();
    void Render();

    virtual void UpdateData() {}

public:
    int GetLayerIndex() { return layer_index_; }
    void AddComponent(CComponent* comp);
    void AddComponent(COMPONENT_TYPE type);
    CComponent* GetComponent(COMPONENT_TYPE type) { return component_array_[static_cast<UINT>(type)]; }
    int GetComponentCount() 
    {
        int count = 0;
        for (const auto& component : component_array_)
        {
            if (nullptr != component)
                ++count;
        }
        return count;
    }
    GET_COMPONENT(Transform, COMPONENT_TYPE::TRANSFORM);
    GET_COMPONENT(MeshRender, COMPONENT_TYPE::MESHRENDER);
    GET_COMPONENT(Camera, COMPONENT_TYPE::CAMERA);
    GET_COMPONENT(Collider2D, COMPONENT_TYPE::COLLIDER2D);
    GET_COMPONENT(Light2D, COMPONENT_TYPE::LIGHT2D);
    GET_COMPONENT(Animator2D, COMPONENT_TYPE::ANIMATOR2D);
    GET_COMPONENT(ParticleSystem, COMPONENT_TYPE::PARTICLESYSTEM);
    GET_COMPONENT(TileMap, COMPONENT_TYPE::TILEMAP);
    GET_COMPONENT(RigidBody2D, COMPONENT_TYPE::RIGIDBODY2D);
    GET_COMPONENT(UIComponent, COMPONENT_TYPE::UI);

    const vector<CScript*>& GetScripts() { return scripts_vector_; }
    CGameObject* GetParent() { return parent_object_; }


    void AddChild(CGameObject* child);
    const vector<CGameObject*> GetChildObjects() { return child_object_vector_; }

    bool IsDead() { return OBJECT_STATE::DEAD == object_state_ ? true : false; }
	OBJECT_STATE GetObjectState() {return object_state_;}

    void ReigsterAsPrefab(const wstring& prefabName);

    void SeparateFromParent();
public:
    CLONE(CGameObject);

private:
    void SetDead(float t) { object_state_ = OBJECT_STATE::RESERVE_DEAD; dead_time = t; }
    bool CheckDead();
private:
    array<CComponent*, static_cast<UINT>(COMPONENT_TYPE::END)> component_array_;
    CGameObject* parent_object_;
    vector<CGameObject*> child_object_vector_;
    vector<CScript*> scripts_vector_;
    
    OBJECT_STATE object_state_;
    float delay_accumulated_time;
    float dead_time;
    int layer_index_;

    friend class CLayer;
    friend class CEventManager;
};

