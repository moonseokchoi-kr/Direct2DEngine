#pragma once
/// <summary>
/// 모든 컴포넌트의 최상위 부모클래스
/// 
/// ECS기반 게임엔진 개발의 시작점
/// 
/// 
/// 작성자: 최문석
/// 
/// 버전 : 
///		1.0 - 기본 틀 작성 2021-09-27
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
	
	static UINT next_id_;	//Entity에 부여될 ID, Enitity를 사용하는 객체가 늘어나면 자동으로 늘어남 생성자에서 관리

private:
	UINT id_; 		//ID를 통해 어떠한 Component인지 알수 있음
	wstring name_; //Component를 부를 이름
};

