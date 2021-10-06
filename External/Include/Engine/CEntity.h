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
	void SetName(const wstring& _name) { name_ = _name; }
	const wstring& GetName() { return name_; }

	UINT GetID() { return id_; }

public:
	virtual void UpdateData() = 0;
private:
	
	static UINT next_id_;	//Entity�� �ο��� ID, Enitity�� ����ϴ� ��ü�� �þ�� �ڵ����� �þ �����ڿ��� ����

private:
	UINT id_; 		//ID�� ���� ��� Component���� �˼� ����
	wstring name_; //Component�� �θ� �̸�
};

