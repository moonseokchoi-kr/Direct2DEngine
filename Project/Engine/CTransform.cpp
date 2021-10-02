#include "pch.h"
#include "CTransform.h"
#include "CConstBuffer.h"

#include "CDevice.h"
#include "CKeyManager.h"
#include "CTimeManager.h"

CTransform::CTransform()
	:CComponent(COMPONENT_TYPE::TRANSFORM)
{
}

CTransform::~CTransform()
{
}

void CTransform::Update()
{
	if (KEY_HOLD(KEY::UP))
	{
		m_pos += Vec3(0.f,fDT * 0.5f,0.f);
	}
	if (KEY_HOLD(KEY::DOWN))
	{
		m_pos -= Vec3(0.f, fDT * 0.5f, 0.f);
	}
	if (KEY_HOLD(KEY::LEFT))
	{
		m_pos -= Vec3(fDT * 0.5f,0.f, 0.f);
	}
	if (KEY_HOLD(KEY::RIGHT))
	{
		m_pos += Vec3(fDT * 0.5f, 0.f, 0.f);
	}
	
}

void CTransform::UpdateData()
{
	CConstBuffer* cb = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	cb->SetData(&m_pos, sizeof(Vec4));
	cb->SetPipelineState(PS_VERTEX);
	cb->UpdateData();
}
