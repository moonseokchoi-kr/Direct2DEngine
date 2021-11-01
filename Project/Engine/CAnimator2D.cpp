#include "pch.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"
CAnimator2D::CAnimator2D()
	:CComponent(COMPONENT_TYPE::ANIMATOR2D)
	,current_animation_(nullptr)
	,animation_repeat_(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& origin)
	:CComponent(origin)
	,current_animation_(nullptr)
	,animation_repeat_(origin.animation_repeat_)
{
	unordered_map<wstring, CAnimation2D*>::const_iterator  iter = origin.animation_map_.begin();

	for (; iter != origin.animation_map_.end(); ++iter)
	{
		CAnimation2D* animation = iter->second->Clone();
		animation->owner_ = this;
		animation_map_.insert(make_pair(iter->first, animation));
	}
// 	if (nullptr != origin.current_animation_)
// 	{
// 		current_animation_ = FindAnimation(origin.current_animation_->GetName());
// 		current_animation_->SetCurrentFrame();
// 		current_animation_->Play();
// 	}
}

CAnimator2D::~CAnimator2D()
{
	Safe_Delete_Map(animation_map_);
}

void CAnimator2D::FinalUpdate()
{
	if (nullptr == current_animation_)
		return;
	current_animation_->FinalUpdate();
	if (current_animation_->IsFinish() && animation_repeat_)
	{
		current_animation_->SetCurrentFrame(0);
		current_animation_->Play();
	}
}

void CAnimator2D::UpdateData()
{
	if (nullptr == current_animation_)
		return;
	current_animation_->UpdateData();
}


void CAnimator2D::CreateAnimation(const wstring& animationName, Ptr<CTexture> atlasTexture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration)
{
	assert(nullptr == FindAnimation(animationName));

	CAnimation2D* anim = new CAnimation2D;
	anim->Create(animationName, atlasTexture, leftTopX, leftTopY, sizeX, sizeY, frameCount, duration);
	anim->owner_ = this;

	animation_map_.insert(make_pair(animationName, anim));
}

CAnimation2D* CAnimator2D::FindAnimation(const wstring& animationName)
{
	unordered_map<wstring, CAnimation2D*>::iterator iter = animation_map_.find(animationName);

	if (iter != animation_map_.end())
		return iter->second;
	return nullptr;
}

void CAnimator2D::Play(const wstring& animationName, UINT startFrame, bool repeat)
{
	CAnimation2D* anim = FindAnimation(animationName);

	if (nullptr == anim)
		current_animation_ = nullptr;
	current_animation_ = anim;
	current_animation_->SetCurrentFrame(startFrame);
	current_animation_->Play();
	animation_repeat_ = repeat;
}
