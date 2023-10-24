#include "pch.h"
#include "CBonusEnterState.h"
#include "CObj.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CLevelManager.h"
#include "CBonusStage.h"
#include "CLevel.h"
#include "CPet.h"
#include "CCamera.h"
#include "CUI.h"
#include "CStage1.h"

CBonusEnterState::CBonusEnterState()
{
}

CBonusEnterState::~CBonusEnterState()
{
}

void CBonusEnterState::final_tick()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//���� ������ �÷��̾�(��Ű)�� �ƴ϶�� ����ó��
	assert(pCookie);

	//===========CBonusEnterState���� ������ ��

	
	
	//-y�������� ���
	Vec2 vPos = pCookie->GetPos();
	vPos.y -= 2.f;
	if (vPos.y <= 0)
		vPos.y = 0;

	pCookie->SetPos(Vec2(vPos.x, vPos.y));

	
	//=============CBonusEnterState�� ��ȯ ����
	
	//������ Bonus Enter Animation�� �����߸� ��ȯ ����
	if (GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusEnter")->IsFinish())
	{
		//���� ������ Bonus ������ �ٲ������ üũ
		CBonusStage* pBonusLevel = dynamic_cast<CBonusStage*>(CLevelManager::GetInst()->GetCurLevel());
		if (pBonusLevel != nullptr)
		{
			GetOwnerFSM()->ChangeState(L"BonusIdle");
		}
	}
}

void CBonusEnterState::Enter()
{
	//� ��Ű���� �˾Ƴ���.
	CPlayer* pCookie = dynamic_cast<CPlayer*>(GetOwnerFSM()->GetOwner());

	//�� off
	pCookie->GetRigidbody()->SetGround(false);
	
	//��Ű�� �߷� ��� off
	pCookie->GetRigidbody()->SetGravityAccel(0.f);

	//���� ������ ����ϱ�
	pCookie->SavePos();

	//��Ű, ��, UI, PlayerSkill Layer �����ؼ� ���ο� ������ �ֱ�?
	CPet* pPet = CLevelManager::GetInst()->GetCurLevel()->GetPet();


	vector<CObj*> vecUI = CLevelManager::GetInst()->GetCurLevel()->GetLayer(LAYER::UI);
	vector<CObj*> vecPlayerSkill = CLevelManager::GetInst()->GetCurLevel()->GetLayer(LAYER::PLAYER_SKILL);

	CLevel* pLevel = CLevelManager::GetInst()->GetLevel((int)LEVEL_TYPE::BONUS);
	
	pLevel->AddObject(pCookie, LAYER::PLAYER);
	pLevel->AddObject(pPet, LAYER::PET);

	for (int i = 0; i < vecUI.size(); i++)
	{
		CUI* pUI = static_cast<CUI*>(vecUI[i]);

		pLevel->AddObject(pUI, LAYER::UI);
	}

	for (int i = 0; i < vecPlayerSkill.size(); i++)
	{
		CObj* pPlayerSkill = static_cast<CUI*>(vecPlayerSkill[i]);

		pLevel->AddObject(pPlayerSkill, LAYER::PLAYER_SKILL);
	}

	static_cast<CBonusStage*>(pLevel)->SetScoreText(static_cast<CStage1*>(CLevelManager::GetInst()->GetCurLevel())->GetScoreText());
	static_cast<CBonusStage*>(pLevel)->SetCoinText(static_cast<CStage1*>(CLevelManager::GetInst()->GetCurLevel())->GetCoinText());

	ChangeLevel(LEVEL_TYPE::BONUS);


	//ī�޶� ����Ʈ -> fade in/out
	CCamera::GetInst()->FadeIn(0.8f);

	//BonusEnter �ִϸ��̼��� ����Ѵ�
	pCookie->GetAnimator()->Play(L"BonusEnter", false);
}

void CBonusEnterState::Exit()
{
	//��Ű�� �߷� �ٽ� ���󺹱�
	GetOwnerFSM()->GetOwner()->GetRigidbody()->SetGravityAccel(500.f);

	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"BonusEnter")->Reset();
}
