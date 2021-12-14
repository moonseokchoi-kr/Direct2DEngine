#include "pch.h"
#include "ResourceViewWidget.h"
#include "WidgetManager.h"
#include "InspectorWidget.h"
#include "HierarchyViewWidget.h"

#include <Engine/CResource.h>
#include <Engine/CResourceManager.h>

ResourceViewWidget::ResourceViewWidget()
	:Widget("resource_view")
{
}

ResourceViewWidget::~ResourceViewWidget()
{
}

void ResourceViewWidget::Init()
{
	Renew();
}

void ResourceViewWidget::Update()
{
	///리소스 변경시 트리 갱신
	if (CResourceManager::GetInst()->IsResourceChanged())
	{
		Renew();
	}
	ImGui::Begin("Resource View", &is_active_);

	tree_.Update();

	UpdateChildren();

	ImGui::End();
}

void ResourceViewWidget::Renew()
{
	tree_.Clear();
	tree_.SetName("resource_view_tree");
	Node* root = tree_.AddItem(nullptr, "Resource", 0);
	const auto& resource_names = magic_enum::enum_names<RESOURCE_TYPE>();
	for (UINT i = 0; i < (UINT)resource_names.size() - 1; ++i)
	{
		string str = string(resource_names[i]);
		Node* currentNode = tree_.AddItem(root, str, 0, true);

		const unordered_map<wstring, CResource*>& resourceMap = CResourceManager::GetInst()->GetResource((RESOURCE_TYPE)i);

		for (const auto& pair : resourceMap)
		{
			string key = WStringToString(pair.first);
			tree_.AddItem(currentNode, key, (DWORD_PTR)pair.second, false);
		}
	}

	tree_.SetClickEvent(TREE_CALLBACK(&ResourceViewWidget::ResourceItemClicked), this);
}

void ResourceViewWidget::ResourceItemClicked(DWORD_PTR item, DWORD_PTR data)
{
	CResource* res = reinterpret_cast<CResource*>(data);

	if (nullptr == res)
		return;
	InspectorWidget* inspector = dynamic_cast<InspectorWidget*>(WidgetManager::GetInst()->FindWidget("inspector_view"));
	inspector->SetTargetResource(res);


	HierarchyViewWidget* hierarchy = dynamic_cast<HierarchyViewWidget*>(WidgetManager::GetInst()->FindWidget("hierarchy_view"));
	hierarchy->ReleaseSelectNode();
}
