#pragma once


class CTimeManager
	:public CSingleton<CTimeManager>
{
	SINGLE(CTimeManager);
private:
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER   m_llFrequency;

	double			m_dDT;	// ������ ������ �ð� �� 
	double			m_dAcc; // 1�� üũ�� ���� ���� �ð�
	UINT			m_iCallCount; // �Լ� ȣ�� ȸ�� üũ
	UINT			m_iFPS;		// �ʴ� ȣ�� Ƚ�� 

	// FPS
	// 1 �����Ӵ� �ð� (Delta Time)
public:
	void Init();
	void Update();
	void Render();

public:
	double GetDT() { return m_dDT; }
	float GetfDT() { return (float)m_dDT; }
};

