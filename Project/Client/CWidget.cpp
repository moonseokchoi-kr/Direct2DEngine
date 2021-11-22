#include "pch.h"
#include "CWidget.h"

CWidget::CWidget(const string& name)
	:name_(name)
	,is_active_(true)
{
}

CWidget::~CWidget()
{
}
