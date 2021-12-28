#pragma once
#include "CEntity.h"
#include "CMaterial.h"
#include "CMesh.h"


class CGameObject;

class GameWidget :
    public CEntity
{
public:
    GameWidget();
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


    void SetOwner(CGameObject* owner) { owner_ = owner; }

    bool IsHovered() { return is_hovered_; }
    bool IsClicked() { return is_mouse_l_release_; }
    
    bool IsSelected() { return is_selected_; }
protected:
	Ptr<CMaterial> material_;
	Ptr<CMesh> mesh_;
private:
    void CheckMouseHovered();
private:
    vector<GameWidget*> child_widget_vector_;
    GameWidget* parent_widget_;
    CGameObject* owner_;
    Vec3 offset_;
    Vec3 offset_scale_;
    Matrix widget_world_matrix_;
    bool is_hovered_;
	bool is_mouse_l_down_; //마우스를 눌렀을떄
	bool is_mouse_l_release_; //마우스를 땟을떄
	bool is_selected_;//마우스로 선택되었을때

    friend class CUIComponent;
};

