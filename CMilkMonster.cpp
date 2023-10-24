#include "pch.h"
#include "CMilkMonster.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CTimeManager.h"
#include "CEffect.h"
#include "CMilkShield.h"
#include "CCollider.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CMilkCookie.h"
#include "CSound.h"

CMilkMonster::CMilkMonster(MonsterSize m_eSize)
	: m_iHp(1)
{

	m_pTex = CResourceManager::GetInst()->LoadTexture(L"MilkCookie_Monster_Atlas", L"texture\\cookie\\MilkCookie_Monster.png");
	SetScale(Vec2(160.f, 160.f));

	CreateCollider();
	CreateAnimator();

	GetCollider()->SetOffsetScale(Vec2(-50.f, 50.f));

	//MonsterIdle
	//GetAnimator()->CreateAnimation(L"MonsterIdle", m_pTex, Vec2(0.f, 0.f), Vec2(219.f, 213.f), Vec2(0.f, 0.f), 5, 0.15f);
	//GetAnimator()->FindAnimation(L"MonsterIdle") ->Save(L"animation\\Cookie\\MilkCookie\\MilkCookie_MonsterIdle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_MonsterIdle.anim");

	//MonsterDead
	//GetAnimator()->CreateAnimation(L"MonsterDead", m_pTex, Vec2(0.f, 213.f), Vec2(219.f, 213.f), Vec2(0.f, 0.f), 7, 0.15f);
	//GetAnimator()->FindAnimation(L"MonsterDead")->Save(L"animation\\Cookie\\MilkCookie\\MilkCookie_MonsterDead.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_MonsterDead.anim");

	
	switch (m_eSize)
	{
	case MonsterSize::SMALL:
	{
		GetAnimator()->SetSizeRatio(1);
		m_pRushSFX = CResourceManager::GetInst()->LoadSound(L"Milk_Evil_Rush_Small", L"sound\\Cookie\\Milk_Evil_Rush_Small.wav");
		m_pExtinctSFX = CResourceManager::GetInst()->LoadSound(L"Milk_Evil_Extinction_Small", L"sound\\Cookie\\Milk_Evil_Extinction_Small.wav");
		m_iHp = 1;
	}
		break;
	case MonsterSize::BIG:
	{
		GetAnimator()->SetSizeRatio(2);
		m_pRushSFX = CResourceManager::GetInst()->LoadSound(L"Milk_Evil_Rush_Big", L"sound\\Cookie\\Milk_Evil_Rush_Big.wav");
		m_pExtinctSFX = CResourceManager::GetInst()->LoadSound(L"Milk_Evil_Extinction_Big", L"sound\\Cookie\\Milk_Evil_Extinction_Big.wav");
		m_iHp = 3;
	}
		break;
	default:
		break;
	}

	GetAnimator()->Play(L"MonsterIdle", true);
}

CMilkMonster::~CMilkMonster()
{

}

void CMilkMonster::tick()
{
	if (m_iHp <= 0)
	{
		if (GetAnimator()->GetCurAnimation()->GetName() != L"MonsterDead")
		{
			//Dead 애니메이션 재생
			GetAnimator()->Play(L"MonsterDead", true);

			//Dead 효과음 재생
			m_pExtinctSFX->Play();
		}
		//애니메이션 재생이 끝나면 Dead처리
		if (GetAnimator()->FindAnimation(L"MonsterDead")->IsFinish())
		{
			SetDead();
			return;
		}
	}

 	m_fAccTime += DT;

	if (!m_bGoBack);
	{
		if (m_iHp > 0)
		{
			Vec2 vPos = GetPos();
			vPos.x -= 300.f * DT;
			SetPos(vPos);
		}
	}

	if (m_bGoBack)
	{
		m_fAccTime = 0;

		Vec2 vPos = GetPos();
		vPos.x += 1300.f * DT;

		CMilkCookie* pCookie = dynamic_cast<CMilkCookie*>(CLevelManager::GetInst()->GetCurLevel()->GetPlayer());

		if (vPos.x >= pCookie->GetPos().x + 650.f)
		{
			vPos.x = pCookie->GetPos().x + 650.f;
			m_bGoBack = false;
		}

		SetPos(vPos);
	}


	CObj::tick();
}

void CMilkMonster::render(HDC _dc)
{
	CObj::render(_dc);
}

void CMilkMonster::BeginOverlap(CCollider* _pOther)
{
	//다이나믹캐스트를 이용하여 충돌한 오브젝트가 MilkShield 클래스인지 확인한다
	CMilkShield* pShield = dynamic_cast<CMilkShield*>(_pOther->GetOwner());
	if (pShield == nullptr)
		return;

	//hp를 1 감소시킨다.
	m_iHp -= 1;
	m_bGoBack = true;

}

void CMilkMonster::OnOverlap(CCollider* _pOther)
{

}

void CMilkMonster::EndOverlap(CCollider* _pOther)
{
}

