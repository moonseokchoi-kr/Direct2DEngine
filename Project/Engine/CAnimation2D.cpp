#include "pch.h"
#include "CAnimation2D.h"

#include "CConstBuffer.h"

#include "CTimeManager.h"
#include "CDevice.h"

#include "CGameObject.h"
#include "CCollider2D.h"
#include "CAnimator2D.h"

CAnimation2D::CAnimation2D()
	:owner_(nullptr)
	, current_frame_(0)
	, animation_finish_(false)
	, accumulated_time_(0.f)
	, animation_repeat_(false)
	, flip_animation_horizon_(false)
	, flip_animation_vertical_(false)
{
	animation_back_board_ = Vec2(100.f, 100.f);
}

CAnimation2D::CAnimation2D(const CAnimation2D& origin)
	:CEntity(origin)
	, owner_(nullptr)
	, current_frame_(0)
	, animation_finish_(false)
	, accumulated_time_(0.f)
	, atlas_texture_(origin.atlas_texture_)
	, animation_back_board_(origin.animation_back_board_)
	, animation_repeat_(origin.animation_repeat_)
	, play_on_start_(origin.play_on_start_)
	, flip_animation_horizon_(origin.flip_animation_horizon_)
	, flip_animation_vertical_(origin.flip_animation_vertical_)
	,frame_vector_(origin.frame_vector_)
{
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::Start()
{
}

void CAnimation2D::LateUpdate()
{
	if (animation_finish_)
		return;
	accumulated_time_ += fDT;
	//현재 애니메이션의 오프셋에 맞게 콜라이더 오프셋 설정
	SetCollisionOffset();
	if (frame_vector_[current_frame_].duration < accumulated_time_)
	{
		++current_frame_;

		if (frame_vector_.size() <= current_frame_)
		{
			current_frame_ = 0;
			animation_finish_ = true;
			accumulated_time_ = 0.f;
			return;
		}
		accumulated_time_ -= frame_vector_[current_frame_].duration;
	}
}

void CAnimation2D::UpdateData()
{
	AnimationData data;
	data = frame_vector_[current_frame_].animation_data;
	data.using_anim = 1;
	data.using_flip_horizon = (UINT)flip_animation_horizon_;
	data.using_flip_vertical = (UINT)flip_animation_vertical_;
	CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CONSTANT_BUFFER_TYPE::ANIMATE2D);
	cb->SetData(&data, sizeof(AnimationData));
	cb->SetPipelineStage(PIPELINE_STAGE::PS_PIXEL);
	cb->UpdateData();

	atlas_texture_->SetPipelineStage(PIPELINE_STAGE::PS_PIXEL, 12);
	atlas_texture_->UpdateData();

}

void CAnimation2D::Create(const wstring& animationName, Ptr<CTexture> texture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration, bool filpHorizon, bool filpVertical)
{
	SetName(animationName);

	atlas_texture_ = texture;

	float width = (float)atlas_texture_->GetWidth();
	float height = (float)atlas_texture_->GetHeight();

	AnimationFrame frame = {};

	for (UINT i = 0; i < frameCount; ++i)
	{
		UINT ltX = leftTopX + (sizeX * i);
		UINT ltY = leftTopY;

		frame.animation_data.size = Vec2((float)sizeX / width, (float)sizeY / height);
		frame.animation_data.full_size = Vec2(animation_back_board_.x / width, animation_back_board_.y/ height);
		frame.animation_data.left_top = Vec2((float)ltX / width, (float)ltY / height);
		frame.animation_data.left_top -= (frame.animation_data.full_size- frame.animation_data.size)/ 2.f;
		frame.duration = duration;

		frame_vector_.push_back(frame);
	}
	flip_animation_horizon_ = filpHorizon;
	flip_animation_vertical_ = filpVertical;
}

void CAnimation2D::CreateFrame(Ptr<CTexture> atlasTexture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration, bool filpHorizon, bool filpVertical)
{

	atlas_texture_ = atlasTexture;

	float width = (float)atlas_texture_->GetWidth();
	float height = (float)atlas_texture_->GetHeight();

	AnimationFrame frame = {};


	UINT ltX = leftTopX;
	UINT ltY = leftTopY;
	//TODO: 최대사이즈에 맞게 렌더링하도록 변경
	frame.animation_data.size = Vec2((float)sizeX / width, (float)sizeY / height);
	frame.animation_data.full_size = Vec2(animation_back_board_.x / width, animation_back_board_.y / height);
	frame.animation_data.left_top = Vec2((float)ltX / width, (float)ltY / height);
	frame.animation_data.left_top -= ((frame.animation_data.full_size - frame.animation_data.size) / 2.f);
	frame.duration = duration;
	flip_animation_horizon_ = filpHorizon;
	flip_animation_vertical_ = filpVertical;
	frame_vector_.push_back(frame);
}

void CAnimation2D::Clear()
{
	frame_vector_.clear();
}

void CAnimation2D::ClearAnim()
{
	CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CONSTANT_BUFFER_TYPE::ANIMATE2D);
	cb->Clear();
}

void CAnimation2D::ClearFrame(int index)
{
	frame_vector_.erase(frame_vector_.begin() + index);
	if (current_frame_)
		current_frame_ -= 1;
	else
		current_frame_ = 0;
}

void CAnimation2D::SetCollisionOffset()
{
	CCollider2D* collision = nullptr;
	if(nullptr != owner_)
		collision = owner_->GetOwner()->Collider2D();
	if (nullptr == collision)
		return;
	collision->SetOffsetPosition(frame_vector_[current_frame_].animation_data.offset);
}

void CAnimation2D::SaveToScene(FILE* file)
{
	CEntity::SaveToScene(file);

	size_t frameCount = frame_vector_.size();
	fwrite(&frameCount, sizeof(size_t), 1, file);

	for (size_t i = 0; i < frame_vector_.size(); ++i)
	{
		fwrite(&frame_vector_[i], sizeof(AnimationFrame), 1, file);
	}

	SaveResReference(atlas_texture_, file);
	
	fwrite(&animation_back_board_, sizeof(Vec3), 1, file);
	fwrite(&animation_repeat_, sizeof(bool), 1, file);
	fwrite(&play_on_start_, sizeof(bool), 1, file);
	fwrite(&flip_animation_horizon_, sizeof(bool), 1, file);
	fwrite(&flip_animation_vertical_, sizeof(bool), 1, file);
	fwrite(&current_frame_, sizeof(int), 1, file);
}

void CAnimation2D::LoadFromScene(FILE* file)
{
	CEntity::LoadFromScene(file);

	size_t frameCount = 0;
	fread(&frameCount, sizeof(size_t), 1, file);

	for (size_t i = 0; i < frameCount; ++i)
	{
		AnimationFrame animFrm = {};
		fread(&animFrm, sizeof(AnimationFrame), 1, file);
		frame_vector_.push_back(animFrm);
	}

	LoadResReference(atlas_texture_, file);

	fread(&animation_back_board_, sizeof(Vec3), 1, file);
	fread(&animation_repeat_, sizeof(bool), 1, file);
	fread(&play_on_start_, sizeof(bool), 1, file);
	fread(&flip_animation_horizon_, sizeof(bool), 1, file);
	fread(&flip_animation_vertical_, sizeof(bool), 1, file);
	fread(&current_frame_, sizeof(int), 1, file);
}

void CAnimation2D::SetCurrentFrameData(const AnimationFrame& data)
{
	AnimationFrame current_frame = GetCurrentFrameData();
	float width = (float)atlas_texture_->GetWidth();
	float height = (float)atlas_texture_->GetHeight();
	current_frame.animation_data.size = data.animation_data.size;
	current_frame.animation_data.full_size = Vec2(animation_back_board_.x/ width,animation_back_board_.y/height);
	
	current_frame.animation_data.left_top = data.animation_data.left_top;
	current_frame.animation_data.left_top -= ((current_frame.animation_data.full_size - current_frame.animation_data.size) / 2.f);
	current_frame.animation_data.offset =data.animation_data.offset;
	current_frame.duration = data.duration;
	frame_vector_[current_frame_] = current_frame;
}

void CAnimation2D::SetBackBoard(Vec2 back_borad)
{
	animation_back_board_ = back_borad;
	for (auto& frame : frame_vector_)
		frame.animation_data.full_size = back_borad;
}
