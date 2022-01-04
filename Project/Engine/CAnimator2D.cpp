#include "pch.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"
CAnimator2D::CAnimator2D()
	:CComponent(COMPONENT_TYPE::ANIMATOR2D)
	,current_animation_(nullptr)


{
}

CAnimator2D::CAnimator2D(const CAnimator2D& origin)
	:CComponent(origin)
	,current_animation_(nullptr)
{
	unordered_map<wstring, CAnimation2D*>::const_iterator  iter = origin.animation_map_.begin();

	for (; iter != origin.animation_map_.end(); ++iter)
	{
		CAnimation2D* animation = iter->second->Clone();
		animation->owner_ = this;
		animation_map_.insert(make_pair(iter->first, animation));
	}
	if (nullptr != origin.current_animation_)
	{
		current_animation_ = FindAnimation(origin.current_animation_->GetName());
		current_animation_->SetCurrentFrame(origin.current_animation_->GetCurrentFrameIndex());
		if(!origin.current_animation_->IsFinish())
			current_animation_->Play();
	}
}

CAnimator2D::~CAnimator2D()
{
	Safe_Delete_Map(animation_map_);
}

void CAnimator2D::Start()
{
	if (nullptr == current_animation_)
		return;
	current_animation_->SetCurrentFrame(0);
	if (current_animation_->play_on_start_)
		current_animation_->Play();
	else
		current_animation_->animation_finish_ = true;
}

void CAnimator2D::LateUpdate()
{
	if (nullptr == current_animation_)
		return;
	current_animation_->LateUpdate();
	if (current_animation_->IsFinish() && current_animation_->animation_repeat_)
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


void CAnimator2D::CreateAnimation(const wstring& animationName, Ptr<CTexture> atlasTexture, UINT leftTopX, UINT leftTopY, UINT sizeX, UINT sizeY, UINT frameCount, float duration, bool flipHorizon, bool flipVertical)
{
	CAnimation2D* anim = FindAnimation(animationName);
	
	if (nullptr == anim)
	{
		anim = new CAnimation2D;
		anim->Create(animationName, atlasTexture, leftTopX, leftTopY, sizeX, sizeY, frameCount, duration, flipHorizon, flipVertical);
		anim->owner_ = this;
		animation_map_.insert(make_pair(animationName, anim));
	}
	else
	{
		anim->Create(animationName, atlasTexture, leftTopX, leftTopY, sizeX, sizeY, frameCount, duration, flipHorizon, flipVertical);
	}
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
	{
		current_animation_ = nullptr;
		return;
	}
		
	current_animation_ = anim;
	current_animation_->SetCurrentFrame(startFrame);
	current_animation_->Play();
	current_animation_->SetRepeat(repeat);
}

void CAnimator2D::AddAnimation(CAnimation2D* animtion)
{
	
	unordered_map<wstring, CAnimation2D*>::iterator iter = animation_map_.find(animtion->GetName());
	if (animation_map_.end() != iter)
	{
		iter->second = animtion;
		return;
	}

		
	animation_map_.insert(make_pair(animtion->GetName(), animtion));
	animtion->owner_ = this;
}

void CAnimator2D::DeleteAnimation(const wstring& animationName)
{
	unordered_map<wstring, CAnimation2D*>::iterator iter = animation_map_.find(animationName);
	unordered_map<wstring, CAnimation2D*>::iterator nextIter;
	if (animation_map_.end() != iter)
	{
		nextIter = animation_map_.erase(iter);
		if (animation_map_.end() == nextIter)
			current_animation_ = animation_map_.begin()->second;
		else
			current_animation_ = nextIter->second;
	}
		
}

Vec2 CAnimator2D::GetFlipData()
{
	UINT flipHorizon = current_animation_->flip_animation_horizon_;
	UINT flipVertical = current_animation_->flip_animation_vertical_;

	return Vec2(flipHorizon,flipVertical);
}

void CAnimator2D::SaveToScene(FILE* file)
{
	CComponent::SaveToScene(file);

	size_t size = animation_map_.size();
	fwrite(&size, sizeof(size_t), 1, file);

	unordered_map<wstring, CAnimation2D*>::iterator iter = animation_map_.begin();
	for (; iter != animation_map_.end(); ++iter)
	{
		iter->second->SaveToScene(file);
	}


	int i = !!current_animation_;
	fwrite(&i, sizeof(int), 1, file);


	if (i)
	{
		SaveWStringToFile(current_animation_->GetName(), file);
	} 


}

void CAnimator2D::LoadFromScene(FILE* file)
{
	CComponent::LoadFromScene(file);

	size_t size = 0;
	fread(&size, sizeof(size_t), 1, file);

	for (size_t i = 0; i < size; ++i)
	{
		CAnimation2D* anim = new CAnimation2D;
		anim->LoadFromScene(file);
		animation_map_.insert(make_pair(anim->GetName(), anim));
	}

	int i = 0;
	fread(&i, sizeof(int), 1, file);

	if (i)
	{
		wstring currnetAnimtionName;
		LoadWStringFromFile(currnetAnimtionName, file);
		current_animation_ = FindAnimation(currnetAnimtionName);
	}
}