#pragma once
#include "ComponentWidget.h"
#include "ComboWidget.h"
class Collider2DWidget
	:public ComponentWidget
{
public:
	Collider2DWidget();
	~Collider2DWidget();
public:
	void Init() override;
	void Update() override;

public:
	void ChangeMesh(DWORD_PTR, DWORD_PTR);
private:
	Vec3 CreateControlPanel(const string& label, Vec3 value);
	ComboWidget meshWidget;
	
};

