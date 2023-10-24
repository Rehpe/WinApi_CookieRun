#include "pch.h"
#include "CMilkSkillState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CRigidbody.h"
#include "CAnimation.h"
#include "CCollider.h"
#include "CMilkCookie.h"
#include "CBackgroundEffect.h"
#include "CMilkShield.h"
#include "CMilkMonster.h"
#include "CCollisionManager.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CSound.h"
#include <time.h>

CMilkSkillState::CMilkSkillState()
	:m_fAccTime(0)
{
}

CMilkSkillState::~CMilkSkillState()
{
}

void CMilkSkillState::final_tick()
{
	//어떤 쿠키인지 알아낸다.
	CMilkCookie* pCookie = dynamic_cast<CMilkCookie*>(GetOwnerFSM()->GetOwner());

	//상태 주인이 우유맛 쿠키가 아니라면 에러처리
	assert(pCookie);

	//스킬 사용 중의 충돌체 설정
	CCollisionManager::GetInst()->Clear();
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::JELLY);
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER_SKILL, LAYER::MONSTER);

	//============MilkSkillState에서 실행할 것
	pCookie->SetOnSummon(true);

	//쿠키 애니메이션
	if (IsPressed(KEY::SPACE))
	{
		Vec2 V = pCookie->GetRigidbody()->GetVelocity();
		pCookie->GetRigidbody()->AddVelocity(Vec2(0.f, -(V.y + 300.f)));
		pCookie->GetAnimator()->Play(L"SkillUp", true);
		return;
	}
	if (IsRelease(KEY::SPACE))
	{
		pCookie->GetAnimator()->Play(L"SkillDown", false);
		return;
	}

	//시간 누적
	m_fAccTime += DT;

	//랜덤시드 설정
	srand((unsigned)time(NULL));
	rand();

	//7마리의 몬스터를 생성시킬 간격(0.9~1.8) 7개를 구한다.
	if (pCookie->GetSkillUsingAccTime() < pCookie->GetSkillMaxTime() - 10.f)
	{
		if (m_vecDuration.empty())
		{
			for (int i = 0; i < 7; i++)
			{
				float f = ((rand() % 9) + 9) / 10.f;
				m_vecDuration.push_back(f);
			}
		}
	}
	if (pCookie->GetSkillUsingAccTime() < pCookie->GetSkillMaxTime() - 2.f && pCookie->GetSkillUsingAccTime() != 0)
	{
		//랜덤한 y 좌표(100~620) 에서 몬스터를 생성한다.
		Vec2 vPos = Vec2(pCookie->GetPos().x + 1000.f, 360.f);

		for (size_t i = m_vecDuration.size() - 1; i >= 0; i--)
		{
			if (m_vecDuration.empty())
				break;

			if (m_fAccTime < m_vecDuration[i])
				break;

			if (m_fAccTime >= m_vecDuration[i])
			{
				if (i == 6 || i == 3)
				{
					//큰 몬스터 생성
					CMilkMonster* pBigMonster = new CMilkMonster(MonsterSize::BIG);
					vPos.y = (rand() % 500) + 100;
					Instantiate(pBigMonster, vPos, LAYER::MONSTER);
				}

				else
				{
					//작은 몬스터 생성
					CMilkMonster* pSmallMonster = new CMilkMonster(MonsterSize::SMALL);
					vPos.y = (rand() % 500) + 100;
					Instantiate(pSmallMonster, vPos, LAYER::MONSTER);
				}

				//생성 후 초기화
				m_fAccTime = 0;
				m_vecDuration.pop_back();
				return;
			}
		}
	}
	

	//방패 애니메이션
	if (pCookie->GetShield()->GetAnimator()->GetCurAnimation()->IsFinish())
	{
		pCookie->GetShield()->GetAnimator()->Play(L"SkillIdle", true);
	}

	//=============MilkSkillState의 전환 조건

	//스킬 사용중이 아니라면
	if (pCookie->IsOnSkill() == false)
	{
		GetOwnerFSM()->ChangeState(L"Run");
		return;
	}
}

void CMilkSkillState::Enter()
{
	//어떤 쿠키인지 알아낸다.
	CMilkCookie* pCookie = dynamic_cast<CMilkCookie*>(GetOwnerFSM()->GetOwner());

	//땅 off
	pCookie->GetRigidbody()->SetGround(false);

	//쿠키의 중력 잠시 낮게
	pCookie->GetRigidbody()->SetGravityAccel(600.f);

	//반투명 백그라운드컬러
	m_pEffect = new CBackgroundEffect;
	m_pEffect->init(BACKGROUND_EFFECT::SEMITRANSPARENT);
	m_pEffect->SetAlphaReduce(false);
	Instantiate(m_pEffect, Vec2(0.f, 0.f), LAYER::BACKGROUND_EFFECT);

	//방패 생성
	m_pMilkShield = new CMilkShield;
	Instantiate(m_pMilkShield, pCookie->GetPos(), LAYER::PLAYER_SKILL);
	m_pMilkShield->GetAnimator()->Play(L"SkillEnter", false);
	dynamic_cast<CMilkCookie*>(pCookie)->SetShield(m_pMilkShield);

	//SkillEnter 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"SkillEnter", false);

	//SkillEnter 효과음을 재생한다.
	pCookie->GetSkillEnterSFX()->Play();

	//방패-몬스터 충돌설정 추가
	ResetCollision();
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER_SKILL, LAYER::MONSTER);
}

void CMilkSkillState::Exit()
{
	//어떤 쿠키인지 알아낸다.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//쿠키의 중력 다시 작용
	pCookie->GetRigidbody()->SetGravityAccel(800.f);

	//백그라운드, 방패 Exit 애니메이션 재생
	m_pEffect->SetDead();
	m_pMilkShield->GetAnimator()->Play(L"SkillExit",false);

	//무적 활성화
	pCookie->SetInvincible(true);

	//SkillExit 애니메이션을 재생한다
	pCookie->GetAnimator()->Play(L"SkillExit", false);
	
	//해당 State에서 사용한 모든 애니메이션을 Reset시킨다
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"SkillEnter")->Reset();
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"SkillUp")->Reset();
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"SkillDown")->Reset();
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"SkillExit")->Reset();

	//남은 몬스터가 있다면 Dead 애니메이션 재생 후 Dead시킴
	vector<CObj*> vecMonsterLayer = CLevelManager::GetInst()->GetCurLevel()->GetLayer(LAYER::MONSTER);
	for (int i = 0; i < vecMonsterLayer.size(); i++)
	{
		CMilkMonster* pMon = static_cast<CMilkMonster*>(vecMonsterLayer[i]);
		pMon->GetAnimator()->Play(L"MonsterDead",false);
	}

	pCookie->SetOnSummon(false);
	
	//충돌체 관계 리셋
	ResetCollision();
}
