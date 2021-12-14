#include "pch.h"
#include "TreeWidget.h"

void Node::Update()
{
    if (use_frame_)
        tree_flags_ |= ImGuiTreeNodeFlags_Framed;
    if (child_vector_.empty() && !use_frame_)
    {
        tree_flags_ |= ImGuiTreeNodeFlags_Leaf;
    }

    bool check_left = false;

    if (ImGui::TreeNodeEx(name_.c_str(), tree_flags_))
    {
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            check_left = true;
        }
        for (Node* child : child_vector_)
        {
            child->Update();
        }
        ImGui::TreePop();
    }
    else
    {
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			check_left = true;
		}
    }

    if (check_left)
        tree_->ExcuteClickedCallback(this);
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
}

void TreeWidget::ExcuteClickedCallback(Node* clickNode)
{
    (instance_->*click_event_)((DWORD_PTR)clickNode, clickNode->data_);
}
