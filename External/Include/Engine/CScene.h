#pragma once
#include "CEntity.h"

class CGameObject;
class CLayer;

class CScene :
    public CEntity
{
public:
	CScene();
	~CScene();
public:
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();
	virtual void UpdateData() {};

public:
	void AddGameObject(CGameObject* object, UINT layerType, bool bMove);

	CLayer* GetLayer(int index) { return layer_array_[index]; }
	CGameObject* GetObjectWithName(const wstring& objectName);
	CScene* Clone() { return nullptr; }

	const wstring& GetScenePath() { return scene_path_; }
	void SetScenePath(const wstring& path) { scene_path_ = path; }
private:
	array<CLayer*,MAX_LAYER> layer_array_;
	wstring scene_path_;
};

