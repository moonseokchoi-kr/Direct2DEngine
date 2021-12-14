#pragma once
#include "Widget.h"

class TreeWidget;
class Node
{
public:
    Node()
        :data_(0)
        ,tree_(nullptr)
        ,parent_(nullptr)
        ,use_frame_(false)
        ,tree_flags_(0)
    {}
	Node(const string& name, DWORD_PTR data, Node* parent, bool use_frame)
		:name_(name)
        ,data_(data)
		, tree_(nullptr)
		, parent_(parent)
		, use_frame_(use_frame)
        , tree_flags_(0)
	{}
    ~Node() {};

public:
    void Update();

private:
	void CheckMouseClick();
	void CheckDragDrop();
public:
	void SetName(const string& str) { name_ = str; }
	const string& GetName() { return name_; }
	void SetData(DWORD_PTR _data)
	{
		data_ = _data;
	}
	DWORD_PTR GetData()
	{
		return data_;
	}

private:
    string name_;
    DWORD_PTR data_;

    vector<Node*> child_vector_;
    TreeWidget* tree_;
    Node* parent_;

    ImGuiTreeNodeFlags tree_flags_;

    bool use_frame_;
    bool has_mouse_l_button_press_;
    bool has_mouse_l_button_clicked_;
    float accumulated_time_;
    friend class TreeWidget;
};

typedef void(Widget::* TREE_CALLBACK)(DWORD_PTR, DWORD_PTR);

class TreeWidget :
    public Widget
{
public:
    TreeWidget();
    ~TreeWidget();
public:
    void Init() override;
    void Update() override;
    
public:
    Node* AddItem(Node* parent, const string& name, DWORD_PTR secondData, bool use_frame = false);
    void SetClickEvent(TREE_CALLBACK callback, Widget* instance)
    {
        click_event_ = callback;
        click_instance_ = instance;
    }
	void SetDragDropEvent(TREE_CALLBACK callback, Widget* instance)
	{
		drag_drop_event_ = callback;
		drag_drop_instance_ = instance;
	}
    void Clear();
    Node* GetSelectedNode() { return selected_node_; }
    void ReleaseSelectNode() { selected_node_ = nullptr; }

    Node* GetDragStartNode() { return drag_start_node_; }
    void SetDragStartNode(Node* node) { drag_start_node_ = node; }

    Node* GetDropNode() { return drop_node_; }
    void SetDropNode(Node* node);
private:
    void ExcuteClickedCallback(Node* clickNode);
    
private:
    Node* root_node_;
    Node* selected_node_;
    Node* drag_start_node_;
    Node* drop_node_;

    bool is_root_visible_;

    TREE_CALLBACK drag_drop_event_;
    Widget* drag_drop_instance_;
    TREE_CALLBACK click_event_;
    Widget* click_instance_;
 

    friend class Node;
};

