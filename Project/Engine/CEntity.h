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
	virtual CEntity* Clone() = 0;
	UINT GetID() { return id_; }

public:
	virtual void UpdateData() = 0;
public:
	virtual void SaveToScene(FILE* file)
	{
		SaveWStringToFile(name_, file);
	}
	virtual void LoadFromScene(FILE* file)
	{
		LoadWStringFromFile(name_, file);
	}
public:
	CEntity& operator = (const CEntity& _other)
	{
		name_ = _other.name_;
		id_ = next_id_++;
	}
private:
	
	static UINT next_id_;	//Entity�� �ο��� ID, Enitity�� ����ϴ� ��ü�� �þ�� �ڵ����� �þ �����ڿ��� ����

private:
	UINT id_; 		//ID�� ���� ��� Component���� �˼� ����
	wstring name_; //Component�� �θ� �̸�
};

