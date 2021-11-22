#include "pch.h"
#include "CMultipleChildrenWidget.h"

CMultipleChildrenWidget::CMultipleChildrenWidget()
	:CWidget("multiple_children")
	,children{}
{
}

CMultipleChildrenWidget::~CMultipleChildrenWidget()
{
	Safe_Delete_Vec(children);
}

void CMultipleChildrenWidget::UpdateChildren()
{
	for (const auto child : children)
	{
		if (nullptr != child &&child->IsActive());
			child->Update();
	}
}
