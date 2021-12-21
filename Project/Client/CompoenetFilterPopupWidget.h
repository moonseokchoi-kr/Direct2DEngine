#pragma once

#include "FilterWidget.h"

class CGameObject;

class CompoenetFilterPopupWidget :
	public Widget
{
public:
	CompoenetFilterPopupWidget();
	~CompoenetFilterPopupWidget();
public:
	void Init() override;
	void Update() override;
	void SetInstance(CGameObject* gameObj) { target_object_ = gameObj; }
public:
	void AddComponent(string selectItem);
private:
	void Clear();
	
private:
	FilterWidget filter_widget_;
	CGameObject* target_object_;
	
};

