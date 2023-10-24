#include "pch.h"
#include "CRigidbody.h"
#include "CTimeManager.h"
#include "CObj.h"

CRigidbody::CRigidbody(CObj* _pOwner)
	: CComponent(_pOwner)
	, m_fMass(1.f)
	, m_bGround(false)
{
}

CRigidbody::CRigidbody(const CRigidbody& _other)
	: CComponent(_other)
	, m_fMass(_other.m_fMass)
	, m_vVelocity(_other.m_vVelocity)
	, m_vForce(_other.m_vForce)
	, m_bGround(false)
{
}

CRigidbody::~CRigidbody()
{
}

void CRigidbody::tick()
{
	//Rigidbody�� �Ϲ� tick�� ������� �ʴ´�.
}

void CRigidbody::final_tick()
{
	//���ݱ��� ���� ���������� ����Ͽ� �����Ѵ�.

	//���ӵ� = �� / ����
	Vec2 vAccel = m_vForce / m_fMass;

	//���ӵ��� �̿��ؼ� �ӵ��� ������Ų��.
	m_vVelocity += vAccel * DT;

	
	//���߿� �ִ� ��� -> �߷� ����
	if (!m_bGround)
	{
		//�߷°��ӵ� ����
		Vec2 vGravityAccel = Vec2(0.f, m_fGravityAccel);
		m_vVelocity += vGravityAccel * DT;
	}

	//�ִ�ӵ� ����
	if (fabsf(m_vMaxVelocity.x) < fabsf(m_vVelocity.x))
	{
		//������ ������ä(1 �Ǵ� -1) ���Ѽӵ��� ���ؼ� ���Ѽӵ���ŭ ������ �ɾ��ش�
		m_vVelocity.x = (m_vVelocity.x / fabsf(m_vVelocity.x)) * fabsf(m_vMaxVelocity.x);
	}

	//if (m_fGravityVLimit < fabsf(m_vVelocity.y))
	//{
	//	m_vVelocity.y = (m_vVelocity.y / fabsf(m_vVelocity.y)) * m_fGravityVLimit;
	//}


	//������Ʈ�� ��ġ�� �˾Ƴ���.
	Vec2 vPos = GetOwner()->GetPos();

	//���� �ӷ¸�ŭ ������Ʈ�� �̵���Ų��.
	vPos.x += m_vVelocity.x * DT;
	vPos.y += m_vVelocity.y * DT;

	GetOwner()->SetPos(vPos);

	//��ġ �ݿ��� ������ ���� 0���� �ʱ�ȭ�Ѵ�.
	m_vForce = Vec2(0.f, 0.f);
	
}

void CRigidbody::render(HDC _dc)
{
}

void CRigidbody::SetGround(bool _b)
{
	m_bGround = _b;
	if (m_bGround)
	{
		//���� �ö󼭸� �Ʒ��� ���ϴ� �ӵ�(�߷�)�� 0���� �����.
		m_vVelocity.y = 0;
	}
}
