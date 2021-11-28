#pragma once
#include "ComponentWidget.h"
class Collider2DWidget
	:public ComponentWidget
{
public:
	Collider2DWidget();
	~Collider2DWidget();
public:
	void Init() override;
	void Update() override;

private:
	Vec3 CreateControlPanel(const string& label, Vec3 value);
};

