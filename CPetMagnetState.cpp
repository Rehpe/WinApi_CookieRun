#include "pch.h"
#include "CPetMagnetState.h"
#include "CObj.h"
#include "CFSM.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPet.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPlayer.h"
#include "CSound.h"
#include "CResourceManager.h"

CPetMagnetState::CPetMagnetState()
{
}

CPetMagnetState::~CPetMagnetState()
{
}

void CPetMagnetState::final_tick()
{
	//============MagnetState���� ������ ��

	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());
	assert(pPet);

	//��Ű�� ��ġ ��������
	CPlayer* pCookie = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();

	//���� �÷��̾��� ���ʿ� �׻� �پ��ִ´�.
	//Vec2 vPos = pCookie->GetPos() - Vec2(150.f, 100.f);
	//pPet->SetPos(vPos);

	//=============IdleState�� ��ȯ ����

	//�ڼ� ������ ���ӽð� ���� ��
	if (!pPet->IsMagnet())
	{
		GetOwnerFSM()->ChangeState(L"Idle");
		return;
	}

	//���ʽ� Ÿ�� ���� ��
	if (pCookie->IsBonus())
	{
		if (pCookie->IsOnSummon())
			return;

		GetOwnerFSM()->ChangeState(L"BonusEnter");
		return;
	}

	
}

void CPetMagnetState::Enter()
{
	//� ������ �˾Ƴ���.
	CPet* pPet = dynamic_cast<CPet*>(GetOwnerFSM()->GetOwner());

	//���� �ƴ϶�� ����ó��
	assert(pPet);

	//Magnet �ִϸ��̼��� ����Ѵ�
	GetOwnerFSM()->GetOwner()->GetAnimator()->Play(L"Magnet", true);

	//Magnet ȿ������ ����Ѵ�
	m_pMagnetSFX = CResourceManager::GetInst()->LoadSound(L"SFX_Magnet", L"sound\\General\\SFX_Magnet.wav");
	m_pMagnetSFX->Play(true);
}

void CPetMagnetState::Exit()
{
	//�ش� State �ִϸ��̼��� Reset��Ų��
	GetOwnerFSM()->GetOwner()->GetAnimator()->FindAnimation(L"Magnet")->Reset();

	//ȿ���� off
	m_pMagnetSFX->Stop();
}
