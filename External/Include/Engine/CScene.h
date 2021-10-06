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
	void AddGameObject(CGameObject* object, UINT layerType);
private:
	array<CLayer*,MAX_LAYER> layers_;
};

