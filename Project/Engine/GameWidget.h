 #pragma once
#include "CEntity.h"
#include "CMaterial.h"
#include "CMesh.h"


class CGameObject;



class GameWidget :
    public CEntity
{
public:
    GameWidget(UI_TYPE type);
    GameWidget(const GameWidget& origin);
    ~GameWidget();
public:
    virtual void Update() = 0;
    virtual void Render();
    void LateUpdate();
    void FinalUpdate();
    void UpdateData();
public:
    const vector<GameWidget*>& GetChildWidgets() { return child_widget_vector_; }
    GameWidget* GetParent() { return parent_widget_; }
    CGameObject* GetOwner() { return owner_; }
    Ptr<CMaterial> GetMaterial() { return material_; }
    Ptr<CMesh> GetMesh() { return mesh_; }
    UI_TYPE GetType() { return ui_type_; }
    Vec3 GetOffset() { return offset_; }
    Vec3 GetOffSetScale() { return offset_scale_; }

    void SetOwner(CGameObject* owner) { owner_ = owner; }
    void SetOffSet(Vec3 pos) { offset_ = pos; }
    void SetOffSetScale(Vec3 scale) { offset_scale_ = scale; }
    bool IsHovered() { return is_hovered_; }
    bool IsClicked() { return is_mouse_l_release_; }
    
    bool IsSelected() { return is_selected_; }
public:
    void AddChild(GameWidget* widget) { child_widget_vector_.push_back(widget); }
    void SaveToScene(FILE* file);
    void LoadFromScene(FILE* file);
protected:
	Ptr<CMaterial> material_;//����
	Ptr<CMesh> mesh_;//����
    vector<GameWidget*> child_widget_vector_;//����
private:
    void CheckMouseHovered();
private:

    GameWidget* parent_widget_;
    CGameObject* owner_;

    Matrix widget_world_matrix_;

	Vec3 offset_; //����
	Vec3 offset_scale_;//����
    UI_TYPE ui_type_;//����


    bool is_hovered_;
	bool is_mouse_l_down_; //���콺�� ��������
	bool is_mouse_l_release_; //���콺�� ������
	bool is_selected_;//���콺�� ���õǾ�����

    friend class CUIComponent;
};

