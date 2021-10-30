#pragma once
#include "singleton.h"

union COLLIDER_ID
{
	struct
	{
		UINT left_id;
		UINT right_id;
	};

	ULONGLONG id;
};

class CCollider2D;

class CCollisionManager :
    public CSingleton<CCollisionManager>
{
	SINGLE(CCollisionManager);

public:
	void Init();
	void Update();
	void CheckLayer(UINT _left, UINT _right);

	/// <summary>
	/// 현재 체크된 그룹을 전부 언체크한 상태로 돌리는 함수
	/// </summary>
	void Reset() { collision_check_array_.fill(0); }

private:
	void collisionLayerUpdate(UINT leftLayer, UINT rightLayer);
	bool isCollision(CCollider2D* leftCollider, CCollider2D* rightCollider);

private:
	unordered_map<ULONGLONG, bool> collider_info_map_;
	array<UINT, MAX_LAYER> collision_check_array_;


};

