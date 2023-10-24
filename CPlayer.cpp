#include "pch.h"
#include "CPlayer.h"
#include "CEngine.h"
#include "CPathManager.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CRigidbody.h"
#include "CFSM.h"
#include "CEffect.h"

#include "CTexture.h"
#include "CResourceManager.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CCollisionManager.h"
#include "CCamera.h"
#include "CRunState.h"
#include "CJumpState.h"
#include "CSlideState.h"
#include "CObstacle.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CBonusStage.h"
#include "CPet.h"
#include "CBonusExitState.h"
#include "CGingerBraveCookie.h"
#include "CMilkCookie.h"
#include "CUI.h"


CPlayer::CPlayer()
	: m_fHp(100.f)
	, m_fSpeed(300.f)
	, m_iJumpCount(2)
	, m_bIsCrashed(false)
	, m_bIsJump(false)
	, m_bBooster(false)
	, m_bBigger(false)
	, m_fVFSDelay(0)
	, m_fBonusAccTime(0)
	, m_bOnSummon(false)
	, m_fHpReduction(2)
{
	CreateCollider();
	CreateAnimator();
	CreateRigidbody();
	CreateFSM();

	m_pBoostVFXTex = CResourceManager::GetInst()->LoadTexture(L"BoostVFX", L"texture\\VFX\\BoostVFX.png");

	m_fBonusMaxTime = 15.f;

	SetScale(Vec2(60.f, 120.f));

	SetMaxHP(110.f);
	SetHP(GetMaxHP());

	//rigidbody ����
	GetRigidbody()->SetVelocityLimit(Vec2(300.f, 200.f));
	GetRigidbody()->SetGravityAccel(900.f);
	GetRigidbody()->SetGravityVelocityLimit(1000.f);

	m_vSavePos = Vec2(100.f, 400.f);
	SetDoNotDestroy(true);

	LoadSFX();
}

CPlayer::CPlayer(const CPlayer& _other)
	: CObj(_other)
	, m_fSpeed(_other.m_fSpeed)
	, m_fHp(_other.m_fHp)
	, m_iJumpCount(_other.m_iJumpCount)
	, m_bIsCrashed(_other.m_bIsCrashed)
	, m_bIsJump(_other.m_bIsJump)
	, m_bBooster(_other.m_bBooster)
	, m_bInvincible(_other.m_bInvincible)
	, m_fInvAccTime(_other.m_fInvAccTime)
	, m_fBoosterAccTime(_other.m_fBoosterAccTime)
	, m_bBigger(_other.m_bBigger)
	, m_fBiggerAccTime(_other.m_fBiggerAccTime)
	, m_bBonus(_other.m_bBonus)
	, m_fBonusAccTime(_other.m_fBonusAccTime)
	, m_vSavePos(_other.m_vSavePos)
{
}


CPlayer::~CPlayer()
{
}


void CPlayer::tick()
{
	if (CLevelManager::GetInst()->GetCurLevel()->GetName() == L"EditorLevel")
	{
		CCollisionManager::GetInst()->Clear();
		CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::GROUND);
		return;
	}


	//��Ű�� ���� ���θ� Ȯ���ϰ�, ���� ON ���¸� 1.5�ʵ��� �浹ü�� ���ش�
	InvincibleCheck(1.5f);
	BiggerCheck(4.f);
	BoosterCheck(4.f);
	BonusCheck(m_fBonusMaxTime);
	SkillCoolDownCheck();
	SkillUsingCheck();

	m_fHp -= m_fHpReduction * DT;

	if (m_fHp <= 0)
		m_fHp = 0;
	else if (m_fHp >= m_fMaxHp)
		m_fHp = m_fMaxHp;
	
	Vec2 vPos = GetPos();
	vPos += Vec2(m_fSpeed, 0.f) * DT;
	SetPos(vPos);
	
	if (GetPos().y >= 800)
	{
		m_fHp = 0;
		m_bOnSkill = false;
		m_bOnSummon = false;
		m_bBonus = false;
		if (GetFSM()->GetCurState()->GetKey() != L"Dead")
		{
			GetFSM()->ChangeState(L"Dead");
		}
	}

	if (IsTap(KEY::Z))
	{
		m_fHp -= 100;
	}
	if (IsTap(KEY::LALT))
	{
		if (m_bBigger == false)
			m_bBigger = true;
	}


	CObj::tick();
}

void CPlayer::render(HDC _dc)
{
	////TextOut(_dc, 0, 20, xyPosText.c_str(), xyPosText.size());	
	//Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());
 //	std::wstring str1 = std::to_wstring(GetCollider()->GetFinalPos().x);
	//std::wstring str2 = std::to_wstring(GetCollider()->GetFinalPos().y);
	//std::wstring str = L"Collider FinalPos" + str1 + L": " + str2;
	//TextOut(_dc, vPos.x, vPos.y - 100.f, str.c_str(), str.size());

	//std::wstring str3 = std::to_wstring(GetPos().x);
	//std::wstring str4 = std::to_wstring(GetPos().y);
	//str = L"Cookie FinalPos" + str1 + L": " + str2;
	//TextOut(_dc, vPos.x, vPos.y - 50.f, str.c_str(), str.size());
	
	CObj::render(_dc);
}

void CPlayer::BeginOverlap(CCollider* _pOther)
{
	
}

void CPlayer::OnOverlap(CCollider* _pOther)
{
}

void CPlayer::EndOverlap(CCollider* _pOther)
{
}

void CPlayer::InvincibleCheck(float _fTime)
{
	//���� ���� ���� �������¶��
	if (m_bInvincible)
	{
		//�ð� ����
		m_fInvAccTime += DT;

		//�浹ü �Ŵ����� �浹���踦 Ground, Jelly�� ���ܳ��´�
		CCollisionManager::GetInst()->Clear();
		CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::GROUND);
		CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::JELLY);

		//�÷��̾� �ִϸ��̼��� �����Ÿ��� �����
		static float fRatio = 0.f;		//����(0�� ����, 1�� ������)
		static float Dir = 1.f;			//������ �ø������� ���� ������
		fRatio += DT * Dir * 8.f;		

		if (1.f < fRatio)
		{
			fRatio = 1.f;
			Dir = -1.f;
		}
		else if (fRatio < 0.f)
		{
			fRatio = 0.f;
			Dir = 1;
		}

		GetAnimator()->SetRatio(fRatio);

		//���� �ð��� ������
		if (m_fInvAccTime >= _fTime)
		{
			//���� ���¸� OFF�ϰ�, �����ð��� 0���� �ʱ�ȭ�Ѵ�.
			m_bInvincible = false;
			m_fInvAccTime = 0;
			
			//�ִϸ��̼��� ���� ������ 1�� �ǵ�����
			GetAnimator()->SetRatio(1);

			//�浹���踦 ������� �ǵ�����
			CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::OBSTACLE);
		}
	}

}

void CPlayer::BoosterCheck(float _fTime)
{
	//�ν��� ���¶��
	if (m_bBooster)
	{
		//�ð� ����
		m_fBoosterAccTime += DT;

		//���� RunState��� Boost �ִϸ��̼��� �÷����Ѵ�.
		if (CState* pState = dynamic_cast<CRunState*>(GetFSM()->GetCurState()))
		{
			if(GetAnimator()->GetCurAnimation()->GetName() == L"Run")
				GetAnimator()->Play(L"Boost", true);
		}

		//�ӵ� ������
		m_fSpeed = 600.f;


		//�Ҳ� ����Ʈ->0.1�ʿ� �ѹ��� ���

		m_fVFSDelay += DT;

		if (m_fVFSDelay >= 0.1f)
		{
			m_fVFSDelay = 0;

			CEffect* pEffect = new CEffect;
			pEffect->SetEffectTex(m_pBoostVFXTex);
			pEffect->SetMaxTime(0.5f);
			pEffect->SetAlphaReduce(true);
			
			Vec2 vPos;
			
			if(!m_bBigger)
				vPos = GetPos() + Vec2(-30.f, -75.f);
			else
				vPos = GetPos() + Vec2(-120.f, -20.f);

			Instantiate(pEffect, vPos, LAYER::BACKGROUND_EFFECT);

			if (m_bBigger)
			{
				pEffect->SetSizeRatio(2);
			}
		}
		 
		//OBSTACLE ���̾�� �浹�� OBSTACLE ���ư��� �ı��ǰ�
		//Obstacle.cpp���� ����

		//���� �ð�
		if (m_fBoosterAccTime >= _fTime)
		{
			//�ν��ͻ��� ����
			m_bBooster = false;

			//���� RunState��� Run �ִϸ��̼��� �÷����Ѵ�.
			if (CState* pState = dynamic_cast<CRunState*>(GetFSM()->GetCurState()))
			{
				GetAnimator()->Play(L"Run", true);
			}

			//���ǵ� �ʱ�ȭ
			m_fSpeed = 300.f;
		
			//�������� ON
			m_bInvincible = true;
			
			//���� �ð� �ʱ�ȭ
			m_fBoosterAccTime = 0;
		}
	}
}

void CPlayer::BiggerCheck(float _fTime)
{
	//�Ŵ�ȭ ���¶��
	if (m_bBigger)
	{
		//�ð� ����
		m_fBiggerAccTime += DT;

		static float fRatio = 0.f;		//ũ�� ����(0�� ����, 1�� 2��)
		fRatio += DT;

		if (fRatio >= 1)
			fRatio = 1;

		GetAnimator()->SetSizeRatio(1 + fRatio);

		GetCollider()->SetSizeRatio(1 + fRatio);

		//Ŀ�� �ִϸ��̼� ������ ����
		GetAnimator()->SetOffset(Vec2(0.f, -140.f));
		if (GetName() == L"SnowSugarCookie")
		{
			GetAnimator()->SetOffset(Vec2(0.f, -170.f));
		}

		//OBSTACLE ���̾�� �浹�� OBSTACLE ���ư��� �ı��ǰ�
		//booster�� ����, obstacle cpp���� ����
		if (m_fBiggerAccTime >= _fTime - 1)
		{
			static float fRatio2 = 1;
			fRatio -= DT;

			if (fRatio <= 0)
				fRatio = 0;

			GetAnimator()->SetSizeRatio((fRatio + 1)); 

			GetCollider()->SetSizeRatio((fRatio + 1));

		}
		//���� �ð� ������
		if (m_fBiggerAccTime >= _fTime)
		{
			//Bigger ���¸� OFF�ϰ�, �����ð��� 0���� �ʱ�ȭ�Ѵ�.
			m_bBigger = false;
			m_fBiggerAccTime = 0;
			
			//�÷��̾�, collider scale ���󺹱�(������)
			GetAnimator()->SetSizeRatio(1);
			GetCollider()->SetSizeRatio(1);
			
			fRatio = 0;

			GetAnimator()->SetOffset(Vec2(0.f, 0.f));
			//�������¸� �Ҵ�
			m_bInvincible = true;
		}
	}
}

void CPlayer::BonusCheck(float _fTime)
{
	if (m_bOnSummon == true)
		return;

	//���ʽ� ���� ���¶��
	if (m_bBonus)
	{
		//�ð� ����
		m_fBonusAccTime += DT;

		//���ʽ� ���� �ִϸ��̼� ȿ��


		//��Ű�� ��ġ ����
		if (GetPos().y <= -10)
		{
			SetPos(Vec2(GetPos().x, -10.f));
		}

		if (GetPos().y >= 700)
		{
			SetPos(Vec2(GetPos().x, 700.f));
		}

		if (m_fBonusAccTime >= _fTime - 0.6)
		{
			if(GetFSM()->GetCurState()->GetKey() != L"BonusExit")
				GetFSM()->ChangeState(L"BonusExit");
		}

		//���� �ð� ������
		if (m_fBonusAccTime >= _fTime)
		{
	
			//Bonus ���¸� OFF�ϰ�, �����ð��� 0���� �ʱ�ȭ�Ѵ�.
			m_bBonus = false;
			m_fBonusAccTime = 0;

			//���̵�ƿ�
			CCamera::GetInst()->FadeIn(0.8f);

			//��Ű, ��, UI, PlayerSkill Layer �����ؼ� ���ο� ������ �ֱ�?
			CPet* pPet = CLevelManager::GetInst()->GetCurLevel()->GetPet();

			vector<CObj*> vecUI = CLevelManager::GetInst()->GetCurLevel()->GetLayer(LAYER::UI);
			vector<CObj*> vecPlayerSkill = CLevelManager::GetInst()->GetCurLevel()->GetLayer(LAYER::PLAYER_SKILL);

			CLevel* pLevel = CLevelManager::GetInst()->GetLevel((int)LEVEL_TYPE::STAGE_01);

			pLevel->AddObject(this, LAYER::PLAYER);
			pLevel->AddObject((CObj*)pPet, LAYER::PET);

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

			

			//���� ü����
			ChangeLevel(LEVEL_TYPE::STAGE_01);

			//�������¸� �Ҵ�
			m_bInvincible = true;
		}
	}
}

void CPlayer::SkillCoolDownCheck()
{
	//��ų ������� �ƴ϶��
	if (!m_bOnSkill)
	{
		//��ų ��Ÿ���� ����
		m_fSkillAccTime += DT;

		if (m_fSkillAccTime >= m_fSkillCoolTime)
		{
			if (!m_bBonus)
			{
				m_bOnSkill = true;
				m_fSkillAccTime = 0.f;
			}
		}
	}
}

void CPlayer::SkillUsingCheck()
{
	//��ų ��� ���̶��
	if (m_bOnSkill)
	{
		//��ų ��� �ð� ����
		m_fSkillUsingAccTime += DT;

		//��ų ��� �ð��� �ִ�ð��� �ٴٸ���

		if (m_fSkillMaxTime != 0 && m_fSkillUsingAccTime >= m_fSkillMaxTime)
		{	
			m_bOnSkill = false;
			m_fSkillUsingAccTime = 0.f;
		}
	}
}

void CPlayer::LoadSFX()
{
	m_pBiggerLandSFX = CResourceManager::GetInst()->LoadSound(L"SFX_BiggerLand", L"sound\\General\\SFX_BiggerLand.wav");
	m_pBiggerJumpSFX = CResourceManager::GetInst()->LoadSound(L"SFX_BiggerJump", L"sound\\General\\SFX_BiggerJump.wav");
	m_pCrashSFX = CResourceManager::GetInst()->LoadSound(L"SFX_Crash", L"sound\\General\\SFX_Crash.wav");
	m_pInvincibleCrashSFX = CResourceManager::GetInst()->LoadSound(L"SFX_InvincibleCrash", L"sound\\General\\SFX_InvincibleCrash.wav");
	m_pBiggerEndSFX = CResourceManager::GetInst()->LoadSound(L"SFX_BiggerEnd", L"sound\\General\\SFX_BiggerEnd.wav");
	m_pGameOverSFX = CResourceManager::GetInst()->LoadSound(L"SFX_GameOver", L"sound\\General\\SFX_GameOver.wav");
}




