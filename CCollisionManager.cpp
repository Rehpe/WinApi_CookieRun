#include "pch.h"
#include "CCollisionManager.h"
#include "CLevel.h"
#include "CLevelManager.h"
#include "CObj.h"
#include "CCollider.h"

CCollisionManager::CCollisionManager()
	: m_ColMatrix{}
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::tick()
{
	for (UINT iRow = 0; iRow < (UINT)LAYER::END; iRow++)
	{
		//��Ʈ������ �»�ܸ� ����� ���̹Ƿ�, iCol�� ���� �ε����� iRow����
		for (UINT iCol = iRow; iCol < (UINT)LAYER::END; iCol++)
		{
			if (!(m_ColMatrix[iRow] & (1 << iCol)))
			{
				//�浹���� �ʴ´�.
				continue;
			}
			//iRow ���̾�� iCol ���̾�� ���� �浹�ϹǷ�, �˻縦 �����Ѵ�.
			CollisionBtwLayer((LAYER)iRow, (LAYER)iCol);
		}
	}
}

void CCollisionManager::CollisionBtwLayer(LAYER _left, LAYER _right)
{
	//���� �Ŵ������� ���� ������ �޾ƿ´�.
	CLevel* pCurLevel = CLevelManager::GetInst()->GetCurLevel();

	//������ ��ȿ������ ���縦 �����ϱ� ����, ��ȯŸ���� ���۷��� Ÿ������ ������ ������ �״�� �����´�.
	//���͸� �Ժη� ������ �� ���� const�� �ٿ��ش�.
	const vector<CObj*>& vecLeft = pCurLevel->GetLayer(_left);
	const vector<CObj*>& vecRight = pCurLevel->GetLayer(_right);

	for (size_t i = 0; i < vecLeft.size(); i++)
	{
		//���̾� �� ������Ʈ���� �浹ü�� ���ٸ� �ǳʶٰ� ���� ������Ʈ�� �˻�
		if (vecLeft[i]->GetCollider() == nullptr)
			continue;

		//���� ���̾� �׷쳢���� �浹 üũ���, j for���� �������� i�� ��� �ߺ� üũ�� �Ͼ�� �ʰ� �Ѵ�.
		size_t j = 0;

		if (_left == _right)
		{
			j = i;
		}

		for (; j < vecRight.size(); j++)
		{
			//���̾� �� ������Ʈ���� �浹ü�� ���ٸ� �ǳʶٰ� ���� ������Ʈ�� �˻�
			//�� ����� ������ ���� ���(���� ���̾� �ȿ��� �浹�˻��ϴ� ���)
			//�ϳ��� ��ü�� �˻��ϴ� ���̹Ƿ� �ǳʶڴ�(�ڽŰ� �ڽų����� �浹�� �� ����)
			if (vecRight[j]->GetCollider() == nullptr || vecLeft[i] == vecRight[j])
				continue;

			//�� ������Ʈ�� ���̵� ������ �� ������Ʈ���� �浹�� ����� �浹id�� �����.
			CollisionID ID = {};
			ID.LeftID = vecLeft[i]->GetCollider()->GetID();
			ID.RightID = vecRight[j]->GetCollider()->GetID();

			//������� ���������, �浹 ���θ� �˻��ϱ� ��, ���� �����ӿ� �浹 ����� �ִ��� üũ
			//map���� key(ID)������ find�� ����� iterator�� ��ȯ�Ǹ�,
			//ã�����ϴ� ���� ���� ��� end ���ͷ����͸� ��ȯ�Ѵ�.
			map<UINT_PTR, bool>::iterator iter = m_mapPrevInfo.find(ID.id);

			//�˻� ����� ���� (�� �浹ü�� ���� �浹�� ���� ����)
			if (iter == m_mapPrevInfo.end())
			{
				//�浹��� �ʿ� �� �浹ü�� �浹�� ���� ���ٰ� ����Ѵ�
				m_mapPrevInfo.insert(make_pair(ID.id, false));
				iter = m_mapPrevInfo.find(ID.id); //false�� �־��� ������ �ٽ� �޾ƿ´�.
			}

			//�� �浹ü�� ���� ��ü �� �ϳ��� Dead ��Ȳ�� ����� bool���� ����
			bool bDead = vecLeft[i]->IsDead() || vecRight[j]->IsDead();

			//�� ������Ʈ�� Collider�� ���� �����ִ�
			if (CollisionBtwCollider(vecLeft[i]->GetCollider(), vecRight[j]->GetCollider()))
			{
				//�������� �������ִ�.
				if (iter->second == true)
				{
					//�浹ü �� �ϳ��� Dead ���¶��
					if (bDead)
					{
						//�浹ü �ϳ��� ��������μ� ��ħ���°� Ǯ�����Ƿ� EndOverlap ȣ��
						vecLeft[i]->GetCollider()->EndOverlap(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->EndOverlap(vecLeft[i]->GetCollider());
					}
					else
					{
						//�� �����ӿ������� ��� �浹�� �Ͼ�� ���̹Ƿ� OnOverlap ȣ��
						vecLeft[i]->GetCollider()->OnOverlap(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->OnOverlap(vecLeft[i]->GetCollider());
					}

				}
				//������ �浹�� ���� ����.
				else
				{
					//�� �� �ϳ��� Dead ���¶��, ������ ���� tick�� �浹 ������Ʈ�� ����� ���̹Ƿ�
					//�浹�� ������ ������ �ϰ� �ƹ� ȣ�⵵ ���� �ʴ´�.

					if (!bDead)
					{
						//�� �����ӿ��� ó�� �浹�� ���۵� ���̹Ƿ� BeginOverlap ȣ��
						vecLeft[i]->GetCollider()->BeginOverlap(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->BeginOverlap(vecLeft[i]->GetCollider());

						//�浹ID�� true(���� �����ӿ��� �浹��)�� ����Ѵ�.
						iter->second = true;
					}
				}
			}
			//�� ������Ʈ�� Collider�� ���� �������ִ�(�浹���� �ƴϴ�)
			else
			{
				//������ �浹�� ����� �ִ�.
				if (iter->second == true)
				{
					//�� �����ӿ��� �浹�ϴٰ� �̹� �����ӿ� ���������Ƿ� EndOverlap ȣ��
					vecLeft[i]->GetCollider()->EndOverlap(vecRight[j]->GetCollider());
					vecRight[j]->GetCollider()->EndOverlap(vecLeft[i]->GetCollider());

					//�浹ID�� false(���� �����ӿ��� �浹���� ����)�� ����Ѵ�.
					iter->second = false;
				}
				//���� �浹�ߵ� �ƴϰ�, ���� �����ӿ��� �浹�� �����ٸ� ���� �߰� ������ ���� �ʴ´�.
			}
		}
	}
}

bool CCollisionManager::CollisionBtwCollider(CCollider* _pLeft, CCollider* _pRight)
{
	Vec2 vLeftPos = _pLeft->GetFinalPos();
	Vec2 vLeftScale = _pLeft->GetFinalScale();

	Vec2 vRightPos = _pRight->GetFinalPos();
	Vec2 vRightScale = _pRight->GetFinalScale();

	//�� ������Ʈ�� �߽���(FinalPos) ���� �Ÿ��� �� ������Ʈ ũ���� ������ ���� ������ �۴ٸ�,
	//�� ������Ʈ�� ���� �ִ� ���̴�.

	if (fabsf(vLeftPos.x - vRightPos.x) > (vLeftScale.x / 2.f + vRightScale.x / 2.f))
		return false;

	if (fabsf(vLeftPos.y - vRightPos.y) > (vLeftScale.y / 2.f + vRightScale.y / 2.f))
		return false;

	return true;
}

void CCollisionManager::LayerCheck(LAYER _left, LAYER _right)
{
	//�浹������ ������, �浹 ��Ʈ������ �� ���̾�� �浹�ϴ� ��� 
	//��Ʈ�������� 1�� �־ �浹���θ� ǥ�����ִ� �Լ�

	//�浹 ��Ʈ������ ���� ����� ����ϱ� ���� ���ڷ� ���� �ΰ��� ���̾� �� 
	//���� ���ڸ� ��(Row)���� �޴´�.
	UINT iRow = (UINT)_left;
	UINT iCol = (UINT)_right;

	if (iRow > iCol)
	{
		UINT iTemp = iCol;
		iCol = iRow;
		iRow = iTemp;
	}

	//��Ʈ ������ |= ���� ���� Ư�� ��Ʈ ����
	//��Ʈ ������ << ��� ��Ʈ�� �������� ������ ����ŭ �̵���Ŵ
	//��, iRow�� �ε����� ���� 2����Ʈ short���� iCol�ڸ��� ��Ʈ�� 1�� �����ϰڴ�.
	m_ColMatrix[iRow] |= (1 << iCol);
}
