#include "pch.h"
#include "CTimeManager.h"

#include "CCore.h"

CTimeManager::CTimeManager()
	: m_currentCount{}
	, m_prevCount{}
	, m_frequency{}
	, m_deltaTime(0.)
	, m_dAcc(0.)
	, m_callCount(0)
	, m_stopped(false)
	, m_stopTime{}
	, m_baseTime{}
	, m_pausedTime{}
{

}

CTimeManager::~CTimeManager()
{

}

void CTimeManager::Init()
{
	// ���� ī��Ʈ
	QueryPerformanceCounter(&m_prevCount);
	m_baseTime = m_prevCount;
	// �ʴ� ī��Ʈ Ƚ��
	QueryPerformanceFrequency(&m_frequency);
	m_countsPerSec = 1.0 / (double)m_frequency.QuadPart;
}

void CTimeManager::Update()
{
	if (m_stopped)
	{
		m_deltaTime = 0.0f;
		return;
	}
	QueryPerformanceCounter(&m_currentCount);

	// ���� �������� ī���ð�, ���� ������ ī���� ���� ���̸� ���Ѵ�.
	m_deltaTime = (double)(m_currentCount.QuadPart - m_prevCount.QuadPart) * (double)m_countsPerSec;

	// ����ī��Ʈ ���� ���簪���� ����(�������� ����� ���ؼ�)
	m_prevCount = m_currentCount;
	
	if (m_deltaTime < 0.0)
	{
		m_deltaTime = 0.0f;
	}

#ifdef _DEBUG
	if (m_deltaTime > (1. / 60.))
		m_deltaTime = (1. / 60.);
#endif
}

void CTimeManager::Render()
{
	++m_callCount;
	m_dAcc += m_deltaTime; // DT ����

	if (m_dAcc >= 1.)
	{
		m_fps = m_callCount;
		m_dAcc = 0.;
		m_callCount = 0;

		wchar_t szBuffer[255] = {}; 
		swprintf_s(szBuffer, L"FPS : %d,  DT : %lf", m_fps, m_deltaTime);
		SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);
	}
}

void CTimeManager::Reset()
{
}

void CTimeManager::Start()
{
	LARGE_INTEGER startTime;
	QueryPerformanceCounter(&startTime);

	if (m_stopped)
	{
		m_pausedTime.QuadPart += (startTime.QuadPart - m_stopTime.QuadPart);

		m_prevCount = startTime;
		m_stopTime = LARGE_INTEGER();
		m_stopped = false;
	}
}

void CTimeManager::Stop()
{
	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);
	if (!m_stopped)
	{
		m_stopTime = currTime;
		m_stopped = true;
	}
}

float CTimeManager::GetTotalTime()
{
	if (m_stopped)
	{
		return(float)(((m_stopTime.QuadPart - m_pausedTime.QuadPart) - m_baseTime.QuadPart) * m_countsPerSec);

	}
	else
	{
		return(float)(((m_currentCount.QuadPart - m_pausedTime.QuadPart) - m_baseTime.QuadPart) * m_countsPerSec);
	}
}
