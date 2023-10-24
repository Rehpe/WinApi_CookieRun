#include "pch.h"
#include "CPet.h"
#include "CPlayer.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CTimeManager.h"
#include "CEngine.h"
#include "CCamera.h"
#include "CJelly.h"
#include "CCollisionManager.h"

CPet::CPet()
	: m_pPlayer(nullptr)
	, m_pPetTex(nullptr)
	, m_bIsMagnet(false)
{
	SetDoNotDestroy(true);

	CreateCollider();
	CreateAnimator();
	CreateFSM();

	CLevelManager::GetInst()->GetCurLevel()->SetPet(this);
	m_pPlayer = CLevelManager::GetInst()->GetCurLevel()->GetPlayer();
	m_vDefaultOffset = Vec2(120.f, 120.f);
	Vec2 vPos = m_pPlayer->GetPos() - m_vDefaultOffset;
	SetPos(vPos);
}

CPet::~CPet()
{
}

void CPet::tick()
{
	MagnetCheck(4.f);
	SkillCoolDownCheck();
	CObj::tick();
}

void CPet::render(HDC _dc)
{
	CObj::render(_dc);
}

void CPet::MagnetCheck(float _fTime)
{
	//�ڼ� ���¶��
	if (m_bIsMagnet)
	{
		//�ð� ����
		m_fMagnetAccTime += DT;

		//Ÿ������Ʈ�� ������
		if (m_fMagnetAccTime < _fTime - 1)
		{
			Vec2 vTargetPos = m_pPlayer->GetPos() + Vec2(200.f, -200.f);
			Vec2 vPos = GetPos();

			Vec2 vDir = vTargetPos - vPos;
			vDir.Normalize();

			vPos.x += vDir.x * 1000.f * DT;
			vPos.y += vDir.y * 1000.f * DT;

		
			if (m_pPlayer->GetPos().x + 198.f <= vPos.x && vPos.x <= m_pPlayer->GetPos().x + 202.f
				&& m_pPlayer->GetPos().y - 198.f <= vPos.y && vPos.y <=m_pPlayer->GetPos().y - 202.f)
			{
				vPos = m_pPlayer->GetPos() + Vec2(200.f, -200.f);
			}

			SetPos(vPos);

			//�ֺ��� ��� ������ �������
			vector<CObj*> vecJelly = CLevelManager::GetInst()->GetCurLevel()->GetVisibleJellyLayer();

			for (size_t i = 0; i < vecJelly.size(); i++)
			{
				CJelly* Jelly = static_cast<CJelly*>(vecJelly[i]);

				Vec2 vJellyPos = Jelly->GetPos();
				Vec2 vJellyDir = vPos - vJellyPos;
				vJellyDir.Normalize();

				vJellyPos.x += vJellyDir.x * 500.f * DT;
				vJellyPos.y += vJellyDir.y * 500.f * DT;

				Jelly->SetPos(vJellyPos);
			}
		}

		if (_fTime - 1 <= m_fMagnetAccTime && m_fMagnetAccTime < _fTime)
		{
			Vec2 vTargetPos = m_pPlayer->GetPos() - Vec2(150.f, 100.f);
			Vec2 vPos = GetPos();

			Vec2 vDir = vTargetPos - vPos;
			vDir.Normalize();

			vPos.x += vDir.x * 300.f * DT;
			vPos.y += vDir.y * 300.f * DT;

			SetPos(vPos);
		}

		CCollisionManager::GetInst()->LayerCheck(LAYER::PET, LAYER::JELLY);

		//���� �ð� ������
		if (m_fMagnetAccTime >= _fTime)
		{
			//Bigger ���¸� OFF�ϰ�, �����ð��� 0���� �ʱ�ȭ�Ѵ�.
			m_bIsMagnet = false;
			m_fMagnetAccTime = 0;
			ResetCollision();
		}
	}
}

void CPet::SkillCoolDownCheck()
{
	if (m_fSkillAccTime == 0 && m_fSkillCoolTime == 0)
		return;

	//��ų ������� �ƴ϶��
	if (!m_bOnSkill)
	{
		//��ų ��Ÿ���� ����
		m_fSkillAccTime += DT;

		if (m_fSkillAccTime >= m_fSkillCoolTime)
		{
			m_bOnSkill = true;
			m_fSkillAccTime = 0.f;
		}
	}
}
