#include "pch.h"
#include "HirarchyViewWidget.h"

#include "WidgetManager.h"
#include "InspectorWidget.h"

#include <Engine/CSceneManager.h>
#include <Engine/CEventManager.h>
#include <Engine/CGameObject.h>
#include <Engine/CLayer.h>
#include <Engine/CScene.h>
HirarchyViewWidget::HirarchyViewWidget()
	:Widget("hirarchy_view")
{
}

HirarchyViewWidget::~HirarchyViewWidget()
{
}

void HirarchyViewWidget::Init()
{
	Renew();
}

void HirarchyViewWidget::Update()
{
	if (CEventManager::GetInst()->HasOccuredObjectEvent())
		Renew();

	if (ImGui::Begin("Hirarchy view", &is_active_))
	{
		
		object_tree_.Update();
		UpdateChildren();

		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}

void HirarchyViewWidget::Renew()
{
	object_tree_.Clear();
	Node* root = object_tree_.AddItem(nullptr, "Scene", 0);
	CScene* currentScene = CSceneManager::GetInst()->GetCurrentScene();
	for (int i = 0; i < (UINT)MAX_LAYER; ++i)
	{
		CLayer* layer = currentScene->GetLayer(i);

		const vector<CGameObject*>& parentObjects = layer->GetParentObjects();
		for (const auto& object : parentObjects)
		{
			AddGameObject(root, object);
		}
	}

	object_tree_.SetClickCallBack(TREE_CALLBACK(&HirarchyViewWidget::ClickGameObject), this);
}

void HirarchyViewWidget::AddGameObject(Node* destItem, CGameObject* object)
{
	string objectName = WStringToString(object->GetName());
	Node* currentNode = object_tree_.AddItem(destItem, objectName, (DWORD_PTR)object, false);

	const vector<CGameObject*>& child_vector = object->GetChildObjects();
	for (const auto& child : child_vector)
	{
		AddGameObject(currentNode, child);
	}
}

void HirarchyViewWidget::ClickGameObject(DWORD_PTR node, DWORD_PTR gameObject)
{
	Node* selectedNode = (Node*)node;
	CGameObject* selectObject = (CGameObject*)gameObject;

	InspectorWidget* inspector = dynamic_cast<InspectorWidget*>(WidgetManager::GetInst()->FindWidget("inspector_view"));
	inspector->SetGameObject(selectObject);
}
