#include "pch.h"
#include "CCameraFollowScript.h"



CCameraFollowScript::CCameraFollowScript()
	:CScript((int)SCRIPT_TYPE::CAMERAFOLLOWSCRIPT)
	, target_(nullptr)
	, target_name_(L"None")
{
	ADD_PARAMETER(target_name_);
}

CCameraFollowScript::~CCameraFollowScript()
{
}

void CCameraFollowScript::Start()
{
	target_ = CSceneManager::GetInst()->GetCurrentScene()->GetObjectWithName(target_name_);
}

void CCameraFollowScript::Update()
{
	Vec3 targetPos;
	if (nullptr != target_)
	{
		targetPos = target_->Transform()->GetPosition();
		targetPos.z = 0.f;
	}
		
	else
		targetPos = GetTransform()->GetPosition();
	
	GetTransform()->SetPosition(targetPos);
}

void CCameraFollowScript::SaveToScene(FILE* file)
{
	__super::SaveToScene(file);
	SaveWStringToFile(target_name_, file);
}

void CCameraFollowScript::LoadFromScene(FILE* file)
{
	__super::LoadFromScene(file);
	LoadWStringFromFile(target_name_, file);
}
