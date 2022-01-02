#include "pch.h"
#include "ObjectFilterWidget.h"

#include <Engine/CSceneManager.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

ObjectFilterWidget::ObjectFilterWidget()
	:Widget("##object_filter")
{
}

ObjectFilterWidget::~ObjectFilterWidget()
{
}

void ObjectFilterWidget::Update()
{
	CScene* scene =  CSceneManager::GetInst()->GetCurrentScene();

	filter_widget_.AddFilterString("None");
	int count = 1;
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* layer = scene->GetLayer(i);
		const vector<CGameObject*> objectVector = layer->GetLayerObjects();
		for (const auto& object : objectVector)
		{
			if (!object->GetName().empty())
			{
				filter_widget_.AddFilterString(WStringToString(object->GetName()));
				if (target_object_name_ == WStringToString(object->GetName()))
					filter_widget_.SetCurrentIndex(count);
				++count;
			}
		}
	}
	filter_widget_.Update();
	target_object_name_ = filter_widget_.GetSelectItem();
	filter_widget_.Clear();
}

