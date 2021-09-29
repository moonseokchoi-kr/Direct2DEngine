#pragma once
/// <summary>
/// ��� ������Ʈ�� �ֻ��� �θ�Ŭ����
/// 
/// ECS��� ���ӿ��� ������ ������
/// 
/// 
/// �ۼ���: �ֹ���
/// 
/// ���� : 
///		1.0 - �⺻ Ʋ �ۼ� 2021-09-27
/// </summary>
class CEntity
{

public:
	CEntity();
	CEntity(const CEntity& _other);
	virtual ~CEntity();
public:
	void SetName(const wstring& _name) { m_name = _name; }
	const wstring& GetName() { return m_name; }

	UINT GetID() { return m_ID; }

public:
	virtual void UpdateData() = 0;
private:
	
	static UINT g_NextId;	//Entity�� �ο��� ID, Enitity�� ����ϴ� ��ü�� �þ�� �ڵ����� �þ �����ڿ��� ����

private:
	UINT m_ID; 		//ID�� ���� ��� Component���� �˼� ����
	wstring m_name; //Component�� �θ� �̸�
};

