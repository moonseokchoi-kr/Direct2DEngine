#include "pch.h"
#include "AnimationWidget.h"
#include "ModalListWidget.h"
#include "WidgetManager.h"
#include "AnimationTool.h"

#include <Engine/CAnimator2D.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CGameObject.h>
AnimationWidget::AnimationWidget()
	:ComponentWidget("Animation",Vec2(0,120))
{
}

AnimationWidget::~AnimationWidget()
{
}

void AnimationWidget::Init()
{
}

void AnimationWidget::Update()
{
	CAnimator2D* animator = GetTarget()->Animator2D();
	CAnimation2D* animation = animator->GetCurrentAnimation();
	Start();
	{
		if (ImGui::BeginTable("animation_spilt", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg, ImVec2(0, 0), 0.f))
		{
			ImGui::TableNextColumn();

			ImGui::Text("Current Animation");
			ImGui::TableNextColumn();
			array<char, 256> str = { 0, };
			WideCharToMultiByte(CP_ACP, 0, animation->GetName().c_str(), -1, str.data(), (int)animation->GetName().size(), nullptr, nullptr);
			if (ImGui::Button(str.data(), ImVec2(200, 0)))
			{
				ModalListWidget* listWidget = dynamic_cast<ModalListWidget*>(WidgetManager::GetInst()->FindWidget("modal_list_widget"));

				array<char, 255> szBuffer = { 0, };
				const unordered_map<wstring, CAnimation2D*>& animationMap = animator->GetAnimations();
				int count = 0;
				for (const auto& pair : animationMap)
				{
					if (!listWidget->isOpen())
					{
						WideCharToMultiByte(CP_ACP, 0, pair.first.c_str(), -1, szBuffer.data(), (int)pair.first.size(), nullptr, nullptr);
						listWidget->AddItem(szBuffer.data());
						if (animation != nullptr && animation->GetName() == pair.first)
							listWidget->SetCurrentIndex(count);
						szBuffer.fill(0);
						++count;
					}
				}

				listWidget->SetCaption("Select Animation");
				listWidget->SetCallbackFunc(this, (MODAL_LIST_CALLBACK)&AnimationWidget::ChangeAnimation);
				listWidget->Activate();
			}
	

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			

			ImGui::Text("Current Frame");
			ImGui::TableNextColumn();
			int frame = animation->GetCurrentFrameIndex();
			if (ImGui::InputInt("##animation_frame", &frame,1,100,ImGuiInputTextFlags_ReadOnly))
			{
				if(frame<0)
					animation->SetCurrentFrame(0);
				else if (animation->GetMaxFrameCount() - 1 > (UINT)frame)
					animation->SetCurrentFrame(animation->GetMaxFrameCount() - 1);
				else
					animation->SetCurrentFrame(frame);
				
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::Text("Setting");

			ImGui::TableNextRow();
		
			bool check_repeat = animation->IsRepeat();
			if (ImGui::Checkbox("repeat", &check_repeat))
			{
				animation->SetRepeat(check_repeat);
			}
			ImGui::SameLine();

			bool check_playOnStart = animation->IsPlayOnStart();
			if (ImGui::Checkbox("play on start", &check_playOnStart))
			{
				animation->SetPlayOnStart(check_playOnStart);
			}
			ImGui::EndTable();
		}
		ImGui::Spacing();

		if (ImGui::Button("Create New", ImVec2(100, 0)))
		{
			AnimationTool* tool = dynamic_cast<AnimationTool*>(WidgetManager::GetInst()->FindWidget("animation_tool"));
			tool->SetTargetAnimator(GetTarget()->Animator2D());
			tool->Activate();
			
		}
	}
	End();
}

void AnimationWidget::ChangeAnimation(DWORD_PTR instance, DWORD_PTR animationName)
{
	ModalListWidget* widget = reinterpret_cast<ModalListWidget*>(instance);

	string name = widget->GetSelectedName();
	array<wchar_t, 256> wStr = { 0, };
	MultiByteToWideChar(CP_ACP, 0, name.c_str(), (int)name.size(), wStr.data(), (int)wStr.size());
	GetTarget()->Animator2D()->Play(wStr.data(), 0, true);
}
