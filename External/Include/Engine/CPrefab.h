#pragma once
#include "CResource.h"


class CGameObject;

typedef void (*SAVE_GAME_OBJECT)(CGameObject*, FILE*);
typedef CGameObject* (*LOAD_GAME_OBJECT)(FILE*);

class CPrefab :
    public CResource
{
public:
    static SAVE_GAME_OBJECT g_save_fucntion_;
    static LOAD_GAME_OBJECT g_load_function_;
public:
    ~CPrefab();

public:
    void UpdateData() override {};
    CGameObject* Instantiate();
    
public:
    HRESULT Save(const wstring& relativePath) override;
    HRESULT Load(const wstring& filePath) override;
private:
	CPrefab();
	CPrefab(CGameObject* gameObject);
private:
    CGameObject* prototype_object_;

    friend class CResourceManager;

};

