#include "pch.h"
#include "CMilkShield.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CMilkCookie.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CEffect.h"
#include "CMilkMonster.h"
#include "CSound.h"

CMilkShield::CMilkShield()
{
	CMilkCookie* pCookie = dynamic_cast<CMilkCookie*>(CLevelManager::GetInst()->GetCurLevel()->GetPlayer());

	//우유맛쿠키가 아니라면 오류
	assert(pCookie);

	m_pTex = CResourceManager::GetInst()->LoadTexture(L"MilkCookie_Shield_Atlas", L"texture\\cookie\\MilkCookie_Shield.png");
	SetScale(Vec2(400.f, 325.f));

	CreateCollider(); 
	CreateAnimator();

	GetCollider()->SetOffsetScale(Vec2(-200.f, 0.f));
	GetCollider()->SetOffsetPos(Vec2(-50.f, 0.f));

	//SkillEnter
	//GetAnimator()->CreateAnimation(L"SkillEnter", m_pTex, Vec2(0.f, 0.f), Vec2(400.f, 325.f), Vec2(0.f, 0.f), 7, 0.15f);
	//GetAnimator()->FindAnimation(L"SkillEnter") ->Save(L"animation\\Cookie\\MilkCookie\\MilkCookie_Shield_SkillEnter.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_Shield_SkillEnter.anim");

	//SkillIdle
	//GetAnimator()->CreateAnimation(L"SkillIdle", m_pTex, Vec2(0.f, 325.f), Vec2(400.f, 325.f), Vec2(0.f, 0.f), 8, 0.1f);
	//GetAnimator()->FindAnimation(L"SkillIdle")->Save(L"animation\\Cookie\\MilkCookie\\MilkCookie_Shield_SkillIdle.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_Shield_SkillIdle.anim");


	//SkillExit
	//GetAnimator()->CreateAnimation(L"SkillExit", m_pTex, Vec2(0.f, 650.f), Vec2(400.f, 325.f), Vec2(0.f, 0.f), 4, 0.15f);
	//GetAnimator()->FindAnimation(L"SkillExit")->Save(L"animation\\Cookie\\MilkCookie\\MilkCookie_Shield_SkillExit.anim");
	GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_Shield_SkillExit.anim");

	//Shield Effect
	m_pEffectTex = CResourceManager::GetInst()->LoadTexture(L"MilkCookie_ShieldVFX_Atlas", L"texture\\cookie\\MilkCookie_Shield_VFX.png");
	
	//Shield SFX
	m_pShieldSFX = CResourceManager::GetInst()->LoadSound(L"Milk_Shield_Bounce", L"sound\\Cookie\\Milk_Shield_Bounce.wav");
}

CMilkShield::~CMilkShield()
{
}

void CMilkShield::tick()
{
	CMilkCookie* pCookie = dynamic_cast<CMilkCookie*>(CLevelManager::GetInst()->GetCurLevel()->GetPlayer());

	Vec2 vPos = pCookie->GetPos() + Vec2 (100.f,-150.f);
	SetPos(vPos);

	if (GetAnimator()->FindAnimation(L"SkillExit")->IsFinish())
	{
		SetDead();
	}

	CObj::tick();
}

void CMilkShield::render(HDC _dc)
{
	CObj::render(_dc);
}

void CMilkShield::BeginOverlap(CCollider* _pOther)
{
	//다이나믹캐스트를 이용하여 충돌한 오브젝트가 MilkMonster 클래스인지 확인한다
	CMilkMonster* pMonster = dynamic_cast<CMilkMonster*>(_pOther->GetOwner());
	if (pMonster == nullptr)
		return;

	//점수 추가
	CMilkCookie* pCookie = dynamic_cast<CMilkCookie*>(CLevelManager::GetInst()->GetCurLevel()->GetPlayer());
	pCookie->AddScore(pCookie->GetShieldScore());

	//이펙트 재생
	m_pVFX = new CEffect;
	m_pVFX->SetEffectTex(m_pEffectTex);
	m_pVFX->CreateAnimator();
	m_pVFX->GetAnimator()->LoadAnimation(L"animation\\Cookie\\MilkCookie\\MilkCookie_ShieldVFX.anim");
	Vec2 vPos = GetPos() + Vec2(100.f, 0.f);
	Instantiate(m_pVFX, vPos, LAYER::EFFECT);
	m_pVFX->GetAnimator()->Play(L"MilkShield_VFX", false);

	//효과음 재생
	m_pShieldSFX->Play();
}

void CMilkShield::OnOverlap(CCollider* _pOther)
{
}

void CMilkShield::EndOverlap(CCollider* _pOther)
{
}
