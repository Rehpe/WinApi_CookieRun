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
		//매트릭스의 좌상단만 사용할 것이므로, iCol의 시작 인덱스는 iRow부터
		for (UINT iCol = iRow; iCol < (UINT)LAYER::END; iCol++)
		{
			if (!(m_ColMatrix[iRow] & (1 << iCol)))
			{
				//충돌하지 않는다.
				continue;
			}
			//iRow 레이어와 iCol 레이어는 서로 충돌하므로, 검사를 진행한다.
			CollisionBtwLayer((LAYER)iRow, (LAYER)iCol);
		}
	}
}

void CCollisionManager::CollisionBtwLayer(LAYER _left, LAYER _right)
{
	//레벨 매니저에서 현재 레벨을 받아온다.
	CLevel* pCurLevel = CLevelManager::GetInst()->GetCurLevel();

	//벡터의 비효율적인 복사를 방지하기 위해, 반환타입을 레퍼런스 타입으로 설정해 원본을 그대로 가져온다.
	//벡터를 함부로 수정할 수 없게 const를 붙여준다.
	const vector<CObj*>& vecLeft = pCurLevel->GetLayer(_left);
	const vector<CObj*>& vecRight = pCurLevel->GetLayer(_right);

	for (size_t i = 0; i < vecLeft.size(); i++)
	{
		//레이어 안 오브젝트에게 충돌체가 없다면 건너뛰고 다음 오브젝트를 검사
		if (vecLeft[i]->GetCollider() == nullptr)
			continue;

		//같은 레이어 그룹끼리의 충돌 체크라면, j for문의 시작점을 i로 잡아 중복 체크가 일어나지 않게 한다.
		size_t j = 0;

		if (_left == _right)
		{
			j = i;
		}

		for (; j < vecRight.size(); j++)
		{
			//레이어 안 오브젝트에게 충돌체가 없다면 건너뛰고 다음 오브젝트를 검사
			//비교 대상이 완전히 같을 경우(같은 레이어 안에서 충돌검사하는 경우)
			//하나의 물체를 검사하는 것이므로 건너뛴다(자신과 자신끼리는 충돌할 수 없음)
			if (vecRight[j]->GetCollider() == nullptr || vecLeft[i] == vecRight[j])
				continue;

			//두 오브젝트의 아이디를 가져와 두 오브젝트간의 충돌을 기록할 충돌id를 만든다.
			CollisionID ID = {};
			ID.LeftID = vecLeft[i]->GetCollider()->GetID();
			ID.RightID = vecRight[j]->GetCollider()->GetID();

			//여기까지 통과했으면, 충돌 여부를 검사하기 전, 이전 프레임에 충돌 기록이 있는지 체크
			//map에서 key(ID)값으로 find한 결과는 iterator로 반환되며,
			//찾고자하는 값이 없을 경우 end 이터레이터를 반환한다.
			map<UINT_PTR, bool>::iterator iter = m_mapPrevInfo.find(ID.id);

			//검색 결과가 없음 (두 충돌체는 전에 충돌한 적이 없다)
			if (iter == m_mapPrevInfo.end())
			{
				//충돌기록 맵에 두 충돌체는 충돌한 적이 없다고 기록한다
				m_mapPrevInfo.insert(make_pair(ID.id, false));
				iter = m_mapPrevInfo.find(ID.id); //false를 넣어준 값으로 다시 받아온다.
			}

			//두 충돌체의 소유 객체 중 하나라도 Dead 상황일 경우의 bool변수 선언
			bool bDead = vecLeft[i]->IsDead() || vecRight[j]->IsDead();

			//두 오브젝트의 Collider가 지금 겹쳐있다
			if (CollisionBtwCollider(vecLeft[i]->GetCollider(), vecRight[j]->GetCollider()))
			{
				//이전에도 겹쳐있있다.
				if (iter->second == true)
				{
					//충돌체 중 하나라도 Dead 상태라면
					if (bDead)
					{
						//충돌체 하나가 사라짐으로서 겹침상태가 풀렸으므로 EndOverlap 호출
						vecLeft[i]->GetCollider()->EndOverlap(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->EndOverlap(vecLeft[i]->GetCollider());
					}
					else
					{
						//전 프레임에서부터 계속 충돌이 일어나는 중이므로 OnOverlap 호출
						vecLeft[i]->GetCollider()->OnOverlap(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->OnOverlap(vecLeft[i]->GetCollider());
					}

				}
				//이전에 충돌한 적이 없다.
				else
				{
					//둘 중 하나라도 Dead 상태라면, 어차피 다음 tick에 충돌 오브젝트는 사라질 것이므로
					//충돌을 없었던 것으로 하고 아무 호출도 하지 않는다.

					if (!bDead)
					{
						//이 프레임에서 처음 충돌이 시작된 것이므로 BeginOverlap 호출
						vecLeft[i]->GetCollider()->BeginOverlap(vecRight[j]->GetCollider());
						vecRight[j]->GetCollider()->BeginOverlap(vecLeft[i]->GetCollider());

						//충돌ID에 true(이전 프레임에서 충돌함)를 기록한다.
						iter->second = true;
					}
				}
			}
			//두 오브젝트의 Collider가 지금 떨어져있다(충돌중이 아니다)
			else
			{
				//이전에 충돌한 기록이 있다.
				if (iter->second == true)
				{
					//전 프레임에서 충돌하다가 이번 프레임에 떨어졌으므로 EndOverlap 호출
					vecLeft[i]->GetCollider()->EndOverlap(vecRight[j]->GetCollider());
					vecRight[j]->GetCollider()->EndOverlap(vecLeft[i]->GetCollider());

					//충돌ID에 false(이전 프레임에서 충돌하지 않음)를 기록한다.
					iter->second = false;
				}
				//지금 충돌중도 아니고, 이전 프레임에도 충돌이 없었다면 굳이 추가 연산을 하지 않는다.
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

	//두 오브젝트의 중심점(FinalPos) 간의 거리가 두 오브젝트 크기의 절반을 더한 값보다 작다면,
	//두 오브젝트는 겹쳐 있는 것이다.

	if (fabsf(vLeftPos.x - vRightPos.x) > (vLeftScale.x / 2.f + vRightScale.x / 2.f))
		return false;

	if (fabsf(vLeftPos.y - vRightPos.y) > (vLeftScale.y / 2.f + vRightScale.y / 2.f))
		return false;

	return true;
}

void CCollisionManager::LayerCheck(LAYER _left, LAYER _right)
{
	//충돌설정이 들어오면, 충돌 매트릭스에 두 레이어끼리 충돌하는 경우 
	//비트연산으로 1을 넣어서 충돌여부를 표시해주는 함수

	//충돌 매트릭스의 왼쪽 상단을 사용하기 위해 인자로 들어온 두개의 레이어 중 
	//작은 숫자를 행(Row)으로 받는다.
	UINT iRow = (UINT)_left;
	UINT iCol = (UINT)_right;

	if (iRow > iCol)
	{
		UINT iTemp = iCol;
		iCol = iRow;
		iRow = iTemp;
	}

	//비트 연산자 |= 변수 안의 특정 비트 설정
	//비트 연산자 << 모든 비트를 왼쪽으로 정해진 수만큼 이동시킴
	//즉, iRow번 인덱스를 가진 2바이트 short변수 iCol자리에 비트를 1로 설정하겠다.
	m_ColMatrix[iRow] |= (1 << iCol);
}
