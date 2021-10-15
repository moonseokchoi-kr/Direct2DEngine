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
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();
	virtual void UpdateData() {};

public:
	void AddGameObject(CGameObject* object, UINT layerType, bool bMove);

	CLayer* GetLayer(int index) { return layer_array_[index]; }
private:
	array<CLayer*,MAX_LAYER> layer_array_;
};

