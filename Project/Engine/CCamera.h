#pragma once
#include "CComponent.h"
class CCamera :
    public CComponent
{
public:
    CCamera();
    ~CCamera();

public:
    
    void Update() override;
    void LateUpdate() override;
    void FinalUpdate() override;
	void UpdateData() override{};
    void Render() override;

	void CalViewMatrix();
	void CalProjectionMatrix();

    void SeperateRenderObject();



    void RenderFoward();
    void RenderParticle();
    void RenderPostEffect();
    
public:
    void SaveToScene(FILE* file) override;
    void LoadFromScene(FILE* file) override;

public:
    void SetProjectionType(PROJECTION_TYPE type) { projection_type_ = type; }
    void CheckAllLayout() { layer_check_out_ = 0xffffffff; }
    void SetMainCamera() { camera_index_ = 0; }

    PROJECTION_TYPE GetProjectionType() { return projection_type_; }
    float GetFarZ() { return far_z_; }
    float GetAovY() { return angle_of_view_y_; }
    float GetScale() { return scale_; }
    void SetScale(float scale) { scale_ = scale; }
    void SetFarZ(float z) { far_z_ = z; }
    void SetAovY(float aov) { angle_of_view_y_ = aov; }
    
public:
    CLONE(CCamera);

private:
    PROJECTION_TYPE projection_type_;
    Matrix view_matrix_;
    Matrix projection_matrix_;
    vector<CGameObject*> forward_render_object_vector_;
    vector<CGameObject*> particle_object_vector_;
    vector<CGameObject*> post_effect_vector_;

    float far_z_;
    float angle_of_view_y_;
    float scale_;

    UINT layer_check_out_;

    int camera_index_;
};

