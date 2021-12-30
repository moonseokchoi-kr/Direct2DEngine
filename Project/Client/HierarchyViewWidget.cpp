#include "pch.h"
#include "HierarchyViewWidget.h"

#include "WidgetManager.h"
#include "InspectorWidget.h"
#include "ResourceViewWidget.h"

#include <Engine/CSceneManager.h>
#include <Engine/CEventManager.h>
#include <Engine/CGameObject.h>
#include <Engine/CKeyManager.h>
#include <Engine/CTransform.h>
#include <Engine/CLayer.h>
#include <Engine/CScene.h>


HierarchyViewWidget::HierarchyViewWidget()
	:Widget("hierarchy_view")
{
}

HierarchyViewWidget::~HierarchyViewWidget()
{
}

void HierarchyViewWidget::Init()
{
	Renew();

}

void HierarchyViewWidget::Update()
{
	if (CEventManager::GetInst()->HasOccuredObjectEvent())
		Renew();

	if (ImGui::Begin("Hierarchy view", &is_active_))
	{
		
		object_tree_.Update();
		UpdateChildren();
		if (ImGui::IsWindowHovered())
			KeyCheck();

		ShowRightPopUp();

		ImGui::End();
	}
	else
	{
		ImGui::End();
	}
}

void HierarchyViewWidget::Renew()
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

	object_tree_.SetClickEvent(TREE_CALLBACK(&HierarchyViewWidget::ClickGameObject), this);
	object_tree_.SetDragDropEvent(TREE_CALLBACK(&HierarchyViewWidget::DropGameObject), this);
}

void HierarchyViewWidget::KeyCheck()
{
	ImGuiIO& io = ImGui::GetIO();
	if (KEY_TAP(KEY::DEL))
	{
		Node* selectNode = object_tree_.GetSelectedNode();
		if (nullptr == selectNode)
			return;
		CGameObject* object = reinterpret_cast<CGameObject*>(selectNode->GetData());
		if (nullptr != object)
		{
			Event evn = {};
			evn.event_type = EVENT_TYPE::DELETE_OBJECT;
			evn.lParam = (DWORD_PTR)object;
			CEventManager::GetInst()->AddEvent(evn);

			InspectorWidget* inspector = dynamic_cast<InspectorWidget*>(WidgetManager::GetInst()->FindWidget("inspector_view"));
			inspector->SetGameObject(nullptr);
		}
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		if (!is_popup_open_)
		{
			ImGui::OpenPopup("Object Menu");
		}
	}


}

void HierarchyViewWidget::AddGameObject(Node* destItem, CGameObject* object)
{
	string objectName = WStringToString(object->GetName());
	Node* currentNode = object_tree_.AddItem(destItem, objectName, (DWORD_PTR)object, false);

	const vector<CGameObject*>& child_vector = object->GetChildObjects();
	for (const auto& child : child_vector)
	{
		AddGameObject(currentNode, child);
	}
}

void HierarchyViewWidget::ClickGameObject(DWORD_PTR node, DWORD_PTR gameObject)
{
	Node* selectedNode = (Node*)node;
	CGameObject* selectObject = (CGameObject*)gameObject;

	InspectorWidget* inspector = dynamic_cast<InspectorWidget*>(WidgetManager::GetInst()->FindWidget("inspector_view"));
	inspector->SetGameObject(selectObject);

	ResourceViewWidget* resourceView = dynamic_cast<ResourceViewWidget*>(WidgetManager::GetInst()->FindWidget("resource_view"));
	resourceView->ReleaseSelectNode();
}

void HierarchyViewWidget::DropGameObject(DWORD_PTR dragStratNode, DWORD_PTR dropNode)
{
	Node* startNode = reinterpret_cast<Node*>(dragStratNode);
	Node* droppedNode = reinterpret_cast<Node*>(dropNode);

	CGameObject* parent = reinterpret_cast<CGameObject*>(startNode->GetData());
	CGameObject* child = reinterpret_cast<CGameObject*>(droppedNode->GetData());

	Event evn = {};
	evn.event_type = EVENT_TYPE::ADD_CHILD;
	evn.lParam = (DWORD_PTR)parent;
	evn.wParam = (DWORD_PTR)child;

	CEventManager::GetInst()->AddEvent(evn);
}

void HierarchyViewWidget::ShowRightPopUp()
{
	bool is_play = SCENE_MODE::STOP == CSceneManager::GetInst()->GetSceneMode();
	if(ImGui::BeginPopup("Object Menu", ImGuiWindowFlags_Popup))
	{
		is_popup_open_ = true;
		if (ImGui::MenuItem("Create New Object", NULL, false, is_play)) 
		{
			CGameObject* object = new CGameObject;
			object->AddComponent(new CTransform);
			wstring name = L"Object" + to_wstring(object->GetID());
			object->SetName(name);

			CEventManager::GetInst()->AddEvent(Event{EVENT_TYPE::CREATE_OBJECT,(DWORD_PTR)object,(DWORD_PTR)0 });
		}
		if (ImGui::MenuItem("Remove all", NULL, false, is_play)) {}
		ImGui::EndPopup();
	}
	else
	{
		is_popup_open_ = false;
		ImGui::CloseCurrentPopup();
	}

}
