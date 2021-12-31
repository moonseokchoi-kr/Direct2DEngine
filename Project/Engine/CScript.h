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
    VEC3,
    VEC4,

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

#define ADD_PARAMETER(var) AddParam({ ToWString(PRINT_VAL_NAME(var)),GetType(var),&var });

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

public:
    void SaveToScene(FILE* file);
    void LoadFromScene(FILE* file);
private:
    void UpdateData()  override {};

protected:
    void CreateObject(CGameObject* object, int layerIndex);
    void DeleteObject(CGameObject* object, float t =0.0f);

    void Instantiate(Ptr<CPrefab> prefab, int layerIndex = 0);
    void Instantiate(Ptr<CPrefab> prefab, Vec3 position, int layerIndex = 0, Vec3 rotation = Vec3(0.f, 0.f, 0.f) );
    

protected:
	virtual void OnCollisionEnter(CGameObject* otherObject);
	virtual void OnCollision(CGameObject* otherObject);
	virtual void OnCollisionExit(CGameObject* otherObject);

protected:
    void AddParam(const ScriptParameter& param) { parameter_vector_.push_back(param); };
	template<typename T>
	SCRIPT_PARAM GetType(T variable);
private:


private:
    int script_type_;
    vector<ScriptParameter> parameter_vector_;
    friend class CCollider2D;
    friend class CBox2DCollider;
    CScript();
};

template<typename T>
inline SCRIPT_PARAM CScript::GetType(T variable)
{
    const type_info& info = typeid(T);
    SCRIPT_PARAM param = SCRIPT_PARAM::END;
	if (info.hash_code() == typeid(int).hash_code())
    {
        param = SCRIPT_PARAM::INT;
    }
    else if (info.hash_code() == typeid(float).hash_code())
    {
        param = SCRIPT_PARAM::FLOAT;
    }
	else if (info.hash_code() == typeid(Vec2).hash_code())
	{
		param = SCRIPT_PARAM::VEC2;
	}
	else if (info.hash_code() == typeid(Vec3).hash_code())
	{
        param = SCRIPT_PARAM::VEC3;
	}
	else if (info.hash_code() == typeid(Vec4).hash_code())
	{
		param = SCRIPT_PARAM::VEC4;
	}
	else if (info.hash_code() == typeid(Ptr<CPrefab>).hash_code())
	{
        param = SCRIPT_PARAM::PREFAB;
	}
    return param;
}