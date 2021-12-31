#include "pch.h"
#include "CCollisionManager.h"


#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"
#include "CBox2DCollider.h"
#include "CSceneManager.h"

CCollisionManager::CCollisionManager()
	:check_layer_before(false)
{

}

CCollisionManager::~CCollisionManager()
{

}


void CCollisionManager::Init()
{
}

void CCollisionManager::Update()
{

	for (UINT row = 0; row < MAX_LAYER; ++row)
		for (UINT col = row; col < collision_check_array_.size(); ++col)
		{
			if (collision_check_array_[row] & (1 << col))
			{
				//üũ�� ��Ʈ�� Ȯ���� �׷��� �����Ѵ�
				collisionLayerUpdate(row, col);
			}
		}
}

void CCollisionManager::CheckLayer(UINT _left, UINT _right)
{
	//�� �������� �׷� Ÿ���� ������,
//ū���� ��(��Ʈ)�� ���
	UINT row = _left;
	UINT col = _right;

	if (row > col)
	{
		row = _right;
		col = _left;
	}

	if (collision_check_array_[row] & (1 << col))
	{
		collision_check_array_[row] &= ~(1 << col);
		collision_check_array_[col] &= ~(1 << row);
	}
	else
	{	//��Ʈ����Ʈ�� ����ŭ �о �������
		collision_check_array_[row] |= (1 << col);
		collision_check_array_[col] |= (1 << row);              
	}
}


void CCollisionManager::collisionLayerUpdate(UINT leftLayer, UINT rightLayer)
{
	CScene* currentScene = CSceneManager::GetInst()->GetCurrentScene();

	const vector<CGameObject*>& vecLeft = currentScene->GetLayer(leftLayer)->GetLayerObjects();
	const vector<CGameObject*>& vecRight = currentScene->GetLayer(rightLayer)->GetLayerObjects();

	unordered_map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (nullptr == vecLeft[i]->Collider2D())
		{
			continue;
		}

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			//�浹ü�� ���ų� �ڱ� �ڽŰ��� �浹
			if (nullptr == vecRight[j]->Collider2D() || vecLeft[i] == vecRight[j])
			{
				continue;
			}

			CCollider2D* leftCol = vecLeft[i]->Collider2D();
			CCollider2D* rightCol = vecRight[j]->Collider2D();

			COLLIDER_ID ID;

			ID.left_id = leftCol->GetID();
			ID.right_id = rightCol->GetID();

			iter = collider_info_map_.find(ID.id);

			//���� �浹������ �̵���϶�
			if (collider_info_map_.end() == iter)
			{
				collider_info_map_.insert(make_pair(ID.id, false));
				iter = collider_info_map_.find(ID.id);
			}


			if (isCollision(leftCol, rightCol))
			{
				if (iter->second)
				{
					//�������� �浹�ϰ� ������(OnCollision)
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						//���� �ϳ��� ���� �����̶�� �浹�� ����
						leftCol->OnCollisionExit(rightCol->GetOwner());
						rightCol->OnCollisionExit(leftCol->GetOwner());
						iter->second = false;
					}
					else
					{
						leftCol->OnCollision(rightCol->GetOwner());
						rightCol->OnCollision(leftCol->GetOwner());
					}
				}

				else
				{
					//���� �� �浹�������
					//�ٵ� ���� ���� ���������̶�� �浹��Ű�� ����
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{
						leftCol->OnCollisionEnter(rightCol->GetOwner());
						rightCol->OnCollisionEnter(leftCol->GetOwner());
						iter->second = true;
					}
				}

			}
			else
			{
				//���� �浹�ϰ� ���� �ʴ�.

				if (iter->second)
				{
					leftCol->OnCollisionExit(rightCol->GetOwner());
					rightCol->OnCollisionExit(leftCol->GetOwner());
					iter->second = false;
				}
			}

		}
	}

	//Box2DCollider
	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (nullptr == vecLeft[i]->Box2DCollider())
		{
			continue;
		}

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			//�浹ü�� ���ų� �ڱ� �ڽŰ��� �浹
			if (nullptr == vecRight[j]->Box2DCollider() || vecLeft[i] == vecRight[j])
			{
				continue;
			}

			CBox2DCollider* leftCol = vecLeft[i]->Box2DCollider();
			CBox2DCollider* rightCol = vecRight[j]->Box2DCollider();

			COLLIDER_ID ID;

			ID.left_id = leftCol->GetID();
			ID.right_id = rightCol->GetID();

			iter = collider_info_map_.find(ID.id);

			//���� �浹������ �̵���϶�
			if (collider_info_map_.end() == iter)
			{
				collider_info_map_.insert(make_pair(ID.id, false));
				iter = collider_info_map_.find(ID.id);
			}


			if (isBox2DColliderContact(leftCol, rightCol))
			{
				if (iter->second)
				{
					//�������� �浹�ϰ� ������(OnCollision)
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						//���� �ϳ��� ���� �����̶�� �浹�� ����
						leftCol->OnCollisionExit(rightCol->GetOwner());
						rightCol->OnCollisionExit(leftCol->GetOwner());
						iter->second = false;
					}
					else
					{
						leftCol->OnCollision(rightCol->GetOwner());
						rightCol->OnCollision(leftCol->GetOwner());
					}
				}

				else
				{
					//���� �� �浹�������
					//�ٵ� ���� ���� ���������̶�� �浹��Ű�� ����
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{
						leftCol->OnCollisionEnter(rightCol->GetOwner());
						rightCol->OnCollisionEnter(leftCol->GetOwner());
						iter->second = true;
					}
				}

			}
			else
			{
				//���� �浹�ϰ� ���� �ʴ�.

				if (iter->second)
				{
					leftCol->OnCollisionExit(rightCol->GetOwner());
					//rightCol->OnCollisionExit(leftCol->GetOwner());
					iter->second = false;
				}
			}

		}
	}
}

void CCollisionManager::Box2dColliderCheckLayer()
{
}

bool CCollisionManager::isCollision(CCollider2D* leftCollider, CCollider2D* rightCollider)
{
	const Matrix& leftColliderWorldMatrix = leftCollider->GetColliderWorldMatrix();
	const Matrix& rightColliderWorldMatrix = rightCollider->GetColliderWorldMatrix();

	static array<Vec3,4> localPositionArray = 
	{
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f),
	};

	array<Vec3,4> projectionAxisArray = {};

	Vec3 centerVertex = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), leftColliderWorldMatrix) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), rightColliderWorldMatrix);

	projectionAxisArray[0] = XMVector3TransformCoord(localPositionArray[1], leftColliderWorldMatrix) - XMVector3TransformCoord(localPositionArray[0], leftColliderWorldMatrix);
	projectionAxisArray[1] = XMVector3TransformCoord(localPositionArray[3], leftColliderWorldMatrix) - XMVector3TransformCoord(localPositionArray[0], leftColliderWorldMatrix);

	projectionAxisArray[2] = XMVector3TransformCoord(localPositionArray[1], rightColliderWorldMatrix) - XMVector3TransformCoord(localPositionArray[0], rightColliderWorldMatrix);
	projectionAxisArray[3] = XMVector3TransformCoord(localPositionArray[3], rightColliderWorldMatrix) - XMVector3TransformCoord(localPositionArray[0], rightColliderWorldMatrix);

	for (int i = 0; i < localPositionArray.size(); ++i)
	{
		projectionAxisArray[i].z = 0.f;
	}

	centerVertex.z = 0.f;

	for (int k = 0; k < localPositionArray.size(); ++k)
	{
		Vec3 axis = projectionAxisArray[k];
		axis.Normalize(); 

		float halfDistance = 0.f;

		for (int i = 0; i < projectionAxisArray.size(); ++i)
		{
			halfDistance += abs(axis.Dot(projectionAxisArray[i]) / 2.f);
		}

		float centerDistance = abs(axis.Dot(centerVertex));

		if (halfDistance < centerDistance)
		{
			return false;
		}
	}

	return true;
}

bool CCollisionManager::isBox2DColliderContact(CBox2DCollider* leftCollider, CBox2DCollider* rightColider)
{
	return true;
}
