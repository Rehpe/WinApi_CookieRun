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
	//Rigidbody는 일반 tick을 사용하지 않는다.
}

void CRigidbody::final_tick()
{
	//지금까지 받은 물리연산을 계산하여 결정한다.

	//가속도 = 힘 / 질량
	Vec2 vAccel = m_vForce / m_fMass;

	//가속도를 이용해서 속도를 증가시킨다.
	m_vVelocity += vAccel * DT;

	
	//공중에 있는 경우 -> 중력 적용
	if (!m_bGround)
	{
		//중력가속도 적용
		Vec2 vGravityAccel = Vec2(0.f, m_fGravityAccel);
		m_vVelocity += vGravityAccel * DT;
	}

	//최대속도 제한
	if (fabsf(m_vMaxVelocity.x) < fabsf(m_vVelocity.x))
	{
		//방향은 유지한채(1 또는 -1) 제한속도를 곱해서 제한속도만큼 제한을 걸어준다
		m_vVelocity.x = (m_vVelocity.x / fabsf(m_vVelocity.x)) * fabsf(m_vMaxVelocity.x);
	}

	//if (m_fGravityVLimit < fabsf(m_vVelocity.y))
	//{
	//	m_vVelocity.y = (m_vVelocity.y / fabsf(m_vVelocity.y)) * m_fGravityVLimit;
	//}


	//오브젝트의 위치를 알아낸다.
	Vec2 vPos = GetOwner()->GetPos();

	//최종 속력만큼 오브젝트를 이동시킨다.
	vPos.x += m_vVelocity.x * DT;
	vPos.y += m_vVelocity.y * DT;

	GetOwner()->SetPos(vPos);

	//위치 반영이 끝나면 힘을 0으로 초기화한다.
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
		//땅에 올라서면 아래로 향하는 속도(중력)을 0으로 만든다.
		m_vVelocity.y = 0;
	}
}
