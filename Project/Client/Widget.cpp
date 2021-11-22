#include "pch.h"
#include "Widget.h"

Widget::Widget(const string& name)
	:name_(name)
	,is_active_(true)
	,children_{}
{
}

Widget::~Widget()
{
	Safe_Delete_Vec(children_);
}

void Widget::UpdateChildren()
{
	for (const auto child : children_)
	{
		if (nullptr != child && child->IsActive())
			child->Update();
	}
}
