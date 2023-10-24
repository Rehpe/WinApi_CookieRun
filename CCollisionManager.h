#pragma once

//충돌 설정한 레이어끼리의 충돌을 검사하고, 충돌 기록을 저장하는 충돌체 관리자
class CCollider;

union CollisionID
{
	struct
	{
		UINT	LeftID;		//4바이트 오브젝트 ID 1
		UINT	RightID;	//4바이트 오브젝트 ID 2
	};
	UINT_PTR	id;			//8바이트 두 오브젝트 간의 충돌을 기록한 충돌 id
};

class CCollisionManager
{
	SINGLETON(CCollisionManager);

private:
	//2바이트(=16비트) short변수 16개면 비트연산으로 레이어 갯수인 16x16개의 충돌 검사가 가능하다.
	//WORD는 Unsinged short의 윈도우 버전
	WORD				m_ColMatrix[(UINT)LAYER::END];		

	//모든 Entity들이 가진 고유의 UINT(4바이트 int) Id 두개를 붙여 8바이트 UINT_PTR(Long Long)
	//두 충돌체의 충돌 기록 key값을 만들고(ex-> 184 202 | true : 184번과 202번 물체는 충돌했었다)
	//두 충돌체가 이전 프레임에 충돌한 적이 있었는지 bool타입으로 기록하는 map 자료구조
	map<UINT_PTR, bool>	m_mapPrevInfo;

public:
	void tick();

public:
	void LayerCheck(LAYER _left, LAYER _right);
	void Clear() { for (int i = 0; i < (UINT)LAYER::END; i++) m_ColMatrix[i] = 0; }

private:
	//두 레이어의 오브젝트들에게 각각 충돌검사 진행 후 충돌한다.
	void CollisionBtwLayer(LAYER _left, LAYER _right);
	//위 함수를 만족한 두 충돌체가 실제로 충돌했는지(겹쳐있는지) 확인해주는 함수
	bool CollisionBtwCollider(CCollider* _pLeft, CCollider* _pRight);
};

