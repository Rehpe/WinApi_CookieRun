#include "pch.h"
#include "CFairyShield.h"
#include "CFairyCookie.h"
#include "CLevelManager.h"
#include "CPlayer.h"
#include "CLevel.h"
#include "CResourceManager.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CJelly.h"
#include "CTimeManager.h"

CFairyShield::CFairyShield()
{
	CFairyCookie* pCookie = dynamic_cast<CFairyCookie*>(CLevelManager::GetInst()->GetCurLevel()->GetPlayer());

	//요정맛쿠키가 아니라면 오류
	assert(pCookie);

	m_pShieldTex = CResourceManager::GetInst()->LoadTexture(L"FairyCookie_Shield_Atlas", L"texture\\cookie\\FairyCookie_Shield.png");
	SetScale(Vec2(150.f, 120.f));

	CreateCollider();
	CreateAnimator();

	//GetCollider()->SetOffsetScale(Vec2(-200.f, 0.f));
	//GetCollider()->SetOffsetPos(Vec2(-50.f, 0.f));

	GetAnimator()->LoadAnimation(L"animation\\Cookie\\FairyCookie\\FairyCookie_Shield.anim");
	GetAnimator()->Play(L"Fairy_Shield", true);

	//Shield SFX
	//m_pShieldSFX = CResourceManager::GetInst()->LoadSound(L"Milk_Shield_Bounce", L"sound\\Cookie\\Milk_Shield_Bounce.wav");
}

CFairyShield::~CFairyShield()
{
}

void CFairyShield::tick()
{
	CFairyCookie* pCookie = dynamic_cast<CFairyCookie*>(CLevelManager::GetInst()->GetCurLevel()->GetPlayer());

	Vec2 vPos = pCookie->GetPos() - Vec2(-5.f, 20.f);
	SetPos(vPos);

	//요정맛 쿠키의 방어막은 약한 자력이 있다
	CPlayer* pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
	vector<CObj*> vecJelly = CLevelManager::GetInst()->GetCurLevel()->GetVisibleJellyLayer();


	for (size_t i = 0; i < vecJelly.size(); i++)
	{
		CJelly* Jelly = static_cast<CJelly*>(vecJelly[i]);

		float a = (fabsf(Jelly->GetPos().x - pPlayer->GetPos().x));
		float b = (fabsf(Jelly->GetPos().y - pPlayer->GetPos().y));

		if ((fabsf(Jelly->GetPos().x - pPlayer->GetPos().x) <= 120) && (fabsf(Jelly->GetPos().y - pPlayer->GetPos().y) <= 120))
		{
			Vec2 vJellyPos = Jelly->GetPos();
			Vec2 vJellyDir = GetPos() - vJellyPos;
			vJellyDir.Normalize();

			vJellyPos.x += vJellyDir.x * 500.f * DT;
			vJellyPos.y += vJellyDir.y * 500.f * DT;

			Jelly->SetPos(vJellyPos);
		}
	}

	if (pCookie->IsCrashed())
	{
		SetDead();
	}

	CObj::tick();
}

void CFairyShield::render(HDC _dc)
{
	CObj::render(_dc);
}
