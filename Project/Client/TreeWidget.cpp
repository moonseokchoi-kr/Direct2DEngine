#include "pch.h"
#include "TreeWidget.h"

#include <Engine/CTimeManager.h>

void Node::Update()
{
    tree_flags_ = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
    if (use_frame_)
        tree_flags_ |= ImGuiTreeNodeFlags_Framed;
    if (child_vector_.empty() && !use_frame_)
    {
        tree_flags_ |= ImGuiTreeNodeFlags_Leaf;
    }
    if (this == tree_->GetSelectedNode())
    {
        tree_flags_ |= ImGuiTreeNodeFlags_Selected;
    }
    char keyNumber[255] = { 0, };
    sprintf_s(keyNumber,"%llu",(long long)this);
    string key = string(name_ + "##" + keyNumber);

    if (ImGui::TreeNodeEx(key.c_str(), tree_flags_))
    {
        CheckDragDrop();

        CheckMouseClick();
       
        for (Node* child : child_vector_)
        {
            child->Update();
        }
        ImGui::TreePop();
    }
    else
    {
        CheckDragDrop();
        CheckMouseClick();
		
    }
	
    if (has_mouse_l_button_clicked_)
    {
        tree_->ExcuteClickedCallback(this);
        has_mouse_l_button_clicked_ = false;
    }
        
}

void Node::CheckMouseClick()
{
    if (ImGui::IsItemHovered())
    {
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			has_mouse_l_button_press_ = true;
		}
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
            has_mouse_l_button_press_ = true;
		}
        if (has_mouse_l_button_press_&&ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            has_mouse_l_button_press_ = false;
            has_mouse_l_button_clicked_ = true;
        }
    }
    else
    {
        has_mouse_l_button_press_ = false;
    }
}

void Node::CheckDragDrop()
{
    //드래그 시작
    if (ImGui::BeginDragDropSource())
    {
        //페이로드의 키가 같아야 드래그앤 드랍 가능
        ImGui::SetDragDropPayload("TEST", nullptr, 0);


        ImGui::EndDragDropSource();
    }
    //드랍대상 처리
    else if (ImGui::BeginDragDropTarget())
    {
        if (ImGui::AcceptDragDropPayload("TEST"))
        {
            accumulated_time_ += fDT;
            if(accumulated_time_>1.f)
                ImGui::SetNextItemOpen(true);
        }
        else
        {
            accumulated_time_ = 0.f;
        }
        ImGui::EndDragDropTarget();
    }
}

TreeWidget::TreeWidget()
    :Widget("tree")
    ,root_node_(nullptr)
{
}

TreeWidget::~TreeWidget()
{
    Clear();
}

void TreeWidget::Init()
{
}

void TreeWidget::Update()
{
    if (nullptr == root_node_)
        return;
    if (is_root_visible_)
        root_node_->Update();
    else
    {
        for (Node* child : root_node_->child_vector_)
        {
            child->Update();
        }
    }
    
}

Node* TreeWidget::AddItem(Node* parent, const string& name, DWORD_PTR secondData, bool use_frame)
{
    assert(!(nullptr == parent && nullptr != root_node_));

    Node* newNode = new Node(name, secondData, parent, use_frame);
    if (nullptr == parent)
    {
        root_node_ = newNode;
        newNode->tree_ = this;
    }
    else
    {
        parent->child_vector_.push_back(newNode);
        newNode->tree_ = this;
    }

    return newNode;
}

void TreeWidget::Clear()
{
    list<Node*> node_vector;
    node_vector.push_back(root_node_);

    while (!node_vector.empty())
    {
        Node* node = node_vector.front();
        node_vector.pop_front();

        if(nullptr == node)
            continue;

        for (Node* child : node->child_vector_)
        {
            node_vector.push_back(child);
        }
        SafeDelete(node);
   }

    root_node_ = nullptr;
    selected_node_ = nullptr;
	drag_drop_event_= nullptr;
	click_event_ = nullptr;
    instance_ = nullptr;
}

void TreeWidget::ExcuteClickedCallback(Node* clickNode)
{
    selected_node_ = clickNode;
    (instance_->*click_event_)((DWORD_PTR)clickNode, clickNode->data_);
}
