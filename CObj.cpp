#include "pch.h"
#include "CObj.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidbody.h"
#include "CFSM.h"
#include "CEventManager.h"
#include "CLevelManager.h"
#include "CLevel.h"

CObj::CObj()
	: m_vPos{}
	, m_vScale{}
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidbody(nullptr)
	, m_pFSM(nullptr)
	, m_bDead(false)
	, m_bDoNotDestroy(false)
{
}

//기본 복사생성자를 사용하지 않고 직접 복사생성자를 구현했을 경우, 꼭 부모 클래스의 생성자를 호출해야한다.
CObj::CObj(const CObj& _other)
	:CEntity(_other)
	, m_vPos(_other.m_vPos)
	, m_vScale(_other.m_vScale)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidbody(nullptr)
	, m_pFSM(nullptr)
	, m_bDead(false)
	, m_bDoNotDestroy(false)
{
	//원본이 해당 컴포넌트를 가지고 있을 때만 복사한다.
	if (_other.m_pCollider != nullptr)
	{
		m_pCollider = _other.m_pCollider->Clone();
		m_pCollider->SetOwner(this);
	}
	if (_other.m_pAnimator != nullptr)
	{
		m_pAnimator = _other.m_pAnimator->Clone();
		m_pAnimator->SetOwner(this);
	}
	if (_other.m_pRigidbody != nullptr)
	{
		m_pRigidbody = _other.m_pRigidbody->Clone();
		m_pRigidbody->SetOwner(this);
	}
	if (_other.m_pFSM != nullptr)
	{
		m_pFSM = _other.m_pFSM->Clone();
		m_pFSM->SetOwner(this);
	}
}

CObj::~CObj()
{
	DEL(m_pCollider);
	DEL(m_pAnimator);
	DEL(m_pRigidbody);
	DEL(m_pFSM);
}

void CObj::tick()
{
	//Component가 있는 오브젝트들이라면
	if (m_pCollider != nullptr)
		m_pCollider->tick();

	if (m_pAnimator != nullptr)
		m_pAnimator->tick();
	
	if (m_pFSM != nullptr)
		m_pFSM->tick();
}

void CObj::final_tick()
{
	//오브젝트를 돌며 컴포넌트의 final tick을 호출한다.
	if (m_pRigidbody != nullptr)
		m_pRigidbody->final_tick();

	if (m_pCollider != nullptr)
		m_pCollider->final_tick();

	if (m_pFSM != nullptr)
		m_pFSM->final_tick();

	if (m_pAnimator != nullptr)
		m_pAnimator->final_tick();
}

void CObj::render(HDC _dc)
{
	//if (m_pCollider != nullptr && CLevelManager::GetInst()->GetCurLevel()->GetColliderRender())
	//	m_pCollider->render(_dc);

	if (m_pAnimator != nullptr)
		m_pAnimator->render(_dc);

	if (m_pCollider != nullptr && CLevelManager::GetInst()->GetCurLevel()->GetColliderRender())
		m_pCollider->render(_dc);
}

void CObj::CreateCollider()
{
	m_pCollider = new CCollider(this);
}

void CObj::CreateAnimator()
{
	m_pAnimator = new CAnimator(this);
}

void CObj::CreateRigidbody()
{
	m_pRigidbody = new CRigidbody(this);
}

void CObj::CreateFSM()
{
	m_pFSM = new CFSM(this);
}

void CObj::BeginOverlap(CCollider* _pOther)
{
}

void CObj::OnOverlap(CCollider* _pOther)
{
}

void CObj::EndOverlap(CCollider* _pOther)
{
}

void CObj::SetDead()
{
	tEvent evn = {};
	evn.eType = EVENT_TYPE::DELETE_OBJECT;
	evn.wParam = (DWORD_PTR)this;

	CEventManager::GetInst()->AddEvent(evn);
}
