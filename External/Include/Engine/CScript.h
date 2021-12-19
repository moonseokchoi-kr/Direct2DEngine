#pragma once
#include "CComponent.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CResourceManager.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CTransform.h"
#include "CCollider2D.h"
#include "CPrefab.h"
#include "CAnimator2D.h"

enum class SCRIPT_PARAM
{
    INT,
    FLOAT,
    VEC2,
    VEC4,
    MATRIX,

    TEXTURE,
    PREFAB,

    END,
};

struct ScriptParameter
{
    wstring name;
    SCRIPT_PARAM param;
    void* data;
};


class CCollider2D;

class CScript :
    public CComponent
{
public:
    CScript(int script_type);
    ~CScript();
public:
    virtual void Start() {};
    void FinalUpdate() final {};


public:
    virtual CScript* Clone() = 0;
public:
	const vector<ScriptParameter>& GetParameter() { return parameter_vector_; }
    int GetScriptType() { return script_type_; }
private:
    void UpdateData()  override {};

protected:
    void CreateObject(CGameObject* object, int layerIndex);
    void DeleteObject(CGameObject* object, float t =0.0f);

    void Instantiate(Ptr<CPrefab> prefab, int layerIndex = 0);
    void Instantiate(Ptr<CPrefab> prefab, Vec3 position, int layerIndex = 0, Vec3 rotation = Vec3(0.f, 0.f, 0.f) );

    void AddParam(const ScriptParameter& _param)
    {
        parameter_vector_.push_back(_param);
    }

protected:
	virtual void OnCollisionEnter(CGameObject* otherObject);
	virtual void OnCollision(CGameObject* otherObject);
	virtual void OnCollisionExit(CGameObject* otherObject);


private:
    int script_type_;
    vector<ScriptParameter> parameter_vector_;
    friend class CCollider2D;
    CScript();
};

