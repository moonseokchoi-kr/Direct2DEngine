#include "pch.h"
#include "CAnimation2D.h"
#include "CConstBuffer.h"

#include "CTimeManager.h"
#include "CDevice.h"

CAnimation2D::CAnimation2D()
	:owner_(nullptr)
	,current_frame_(0)
	,animation_finish_(false)
	,accumulated_time_(0.f)
{
}

CAnimation2D::CAnimation2D(const CAnimation2D& origin)
	:owner_(nullptr)
	,current_frame_(0)
	,animation_finish_(false)
	,accumulated_time_(0.f)
{
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::FinalUpdate()
{
	if (animation_finish_)
		return;
	accumulated_time_ += fDT;

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
	CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CONSTANT_BUFFER_TYPE::ANIMATE2D);
	cb->SetData(&data, sizeof(AnimationData));
	cb->SetPipelineStage(PIPELINE_STAGE::PS_PIXEL);
	cb->UpdateData();

	atlas_texture_->SetPipelineStage(PIPELINE_STAGE::PS_PIXEL, 12);
	atlas_texture_->UpdateData();
}

void CAnimation2D::Create(const wstring& animationName, Ptr<CTexture> texture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration)
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
		frame.animation_data.full_size = frame.animation_data.size * 2.f;

		frame.animation_data.left_top = Vec2((float)ltX / width, (float)ltY / height);
		frame.animation_data.left_top -= frame.animation_data.size / 2.f;

		frame.duration = duration;

		frame_vector_.push_back(frame);
	}
}

void CAnimation2D::Clear()
{
	frame_vector_.clear();
}

void CAnimation2D::ClearFrame(int index)
{
	frame_vector_.erase(frame_vector_.begin() + index);
	if (current_frame_)
		current_frame_ -= 1;
	else
		current_frame_ = 0;
}

void CAnimation2D::SetCurrentFrameData(const AnimationFrame& data)
{
	AnimationFrame current_frame = GetCurrentFrameData();
	
	current_frame.animation_data.size = data.animation_data.size;
	current_frame.animation_data.full_size = data.animation_data.size * 2.f;
	
	current_frame.animation_data.left_top = data.animation_data.left_top;
	current_frame.animation_data.left_top -= data.animation_data.size / 2.f;

	current_frame.animation_data.offset =data.animation_data.offset;
	current_frame.duration = data.duration;
	
	frame_vector_[current_frame_] = current_frame;
}