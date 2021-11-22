#include "pch.h"
#include "CSingleChildWidget.h"

CSingleChildWidget::CSingleChildWidget()
	:child(nullptr)
	,CWidget("single_child")
{
}

CSingleChildWidget::~CSingleChildWidget()
{
}

void CSingleChildWidget::UpdateChild()
{
	if (nullptr != child && child->IsActive());
		child->Update();
}
