#pragma once

#include "imgui.h"

class Widget
{
public:
	Widget(const string& name);
	~Widget();

public:
	virtual void Update() = 0;

public:
	void Activate() { is_active_ = true; }
	void Deactivate() { is_active_ = false; }
	bool IsActive() { return is_active_; }

	const string& GetName() { return name_; }

protected:
	bool is_active_;

	void UpdateChildren();

private:
	string name_;
	vector<Widget*> children_;

};

