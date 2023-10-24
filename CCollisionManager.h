#pragma once

//�浹 ������ ���̾���� �浹�� �˻��ϰ�, �浹 ����� �����ϴ� �浹ü ������
class CCollider;

union CollisionID
{
	struct
	{
		UINT	LeftID;		//4����Ʈ ������Ʈ ID 1
		UINT	RightID;	//4����Ʈ ������Ʈ ID 2
	};
	UINT_PTR	id;			//8����Ʈ �� ������Ʈ ���� �浹�� ����� �浹 id
};

class CCollisionManager
{
	SINGLETON(CCollisionManager);

private:
	//2����Ʈ(=16��Ʈ) short���� 16���� ��Ʈ�������� ���̾� ������ 16x16���� �浹 �˻簡 �����ϴ�.
	//WORD�� Unsinged short�� ������ ����
	WORD				m_ColMatrix[(UINT)LAYER::END];		

	//��� Entity���� ���� ������ UINT(4����Ʈ int) Id �ΰ��� �ٿ� 8����Ʈ UINT_PTR(Long Long)
	//�� �浹ü�� �浹 ��� key���� �����(ex-> 184 202 | true : 184���� 202�� ��ü�� �浹�߾���)
	//�� �浹ü�� ���� �����ӿ� �浹�� ���� �־����� boolŸ������ ����ϴ� map �ڷᱸ��
	map<UINT_PTR, bool>	m_mapPrevInfo;

public:
	void tick();

public:
	void LayerCheck(LAYER _left, LAYER _right);
	void Clear() { for (int i = 0; i < (UINT)LAYER::END; i++) m_ColMatrix[i] = 0; }

private:
	//�� ���̾��� ������Ʈ�鿡�� ���� �浹�˻� ���� �� �浹�Ѵ�.
	void CollisionBtwLayer(LAYER _left, LAYER _right);
	//�� �Լ��� ������ �� �浹ü�� ������ �浹�ߴ���(�����ִ���) Ȯ�����ִ� �Լ�
	bool CollisionBtwCollider(CCollider* _pLeft, CCollider* _pRight);
};

