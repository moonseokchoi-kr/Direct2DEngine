#pragma once


class CTimeManager
	:public CSingleton<CTimeManager>
{
	SINGLE(CTimeManager);

public:
	void Init();
	void Update();
	void Render();

	void Reset();
	void Start();
	void Stop();
public:
	double GetDT() { return m_deltaTime; }
	float GetfDT() { return (float)m_deltaTime; }

	float GetTotalTime();
private:
	LARGE_INTEGER	m_currentCount; //����ð�
	LARGE_INTEGER	m_prevCount; //�����ð�
	LARGE_INTEGER   m_frequency;
	
	
	double	m_countsPerSec; //�ʴ� ī��Ʈ��

	double			m_deltaTime;	// ������ ������ �ð� �� 
	double			m_dAcc; // 1�� üũ�� ���� ���� �ð�

	LARGE_INTEGER			m_baseTime;
	LARGE_INTEGER			m_pausedTime;
	LARGE_INTEGER			m_stopTime;

	bool			m_stopped;

	UINT			m_callCount; // �Լ� ȣ�� ȸ�� üũ
	UINT			m_fps;		// �ʴ� ȣ�� Ƚ�� 

	// FPS
	// 1 �����Ӵ� �ð� (Delta Time)
};

