#include "pch.h"
#include "CObstacle.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CCamera.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CPlayer.h"
#include "CTimeManager.h"
#include "CSound.h"

CObstacle::CObstacle()
	:m_bParabola(false)
	, m_iLifeTime(0)
{
	
}

CObstacle::~CObstacle()
{
}


void CObstacle::init(OBS_TYPE _eObsType, bool _bIsPassing)
{
	m_bIsPassing = _bIsPassing;
	m_eObsType = _eObsType;

	switch (_eObsType)
	{
	case OBS_TYPE::FIXED:
	{
		SetScale(Vec2(m_pTexture->GetWidth(), m_pTexture->GetHeight()));
		CMapTile::init();
	}
		break;
	case OBS_TYPE::ANIM_1:
	{
		//�ִϸ����� ����
		CreateAnimator();

		//�ִϸ��̼� ��Ʋ�� �ҷ�����
		CTexture* pAnimTex = CResourceManager::GetInst()->LoadTexture(L"anim_Obs1_Atlas", L"texture\\map\\mapTile\\Anim_Obs\\anim_obs1.png");
		SetTexture(pAnimTex);

		//�浹ü ũ�� ����
		SetScale(Vec2(173.f, 390.f));
		GetCollider()->SetScale(GetScale());
		//�ʿ�� �浹ü ������ �߰�
		GetCollider()->SetOffsetScale(Vec2(-100.f,0.f));
		GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));

		GetAnimator()->LoadAnimation(L"animation\\mapTile\\Anim_Obs1.anim");
		GetAnimator()->Play(L"anim_Obs1_Idle", true);

	}
		break;
	case OBS_TYPE::ANIM_2:
	{
		//�ִϸ����� ����
		CreateAnimator();

		//�ִϸ��̼� ��Ʋ�� �ҷ�����
		CTexture* pAnimTex = CResourceManager::GetInst()->LoadTexture(L"anim_obs2_Atlas", L"texture\\map\\mapTile\\Anim_Obs\\anim_obs2.png");
		SetTexture(pAnimTex);

		//�浹ü ũ�� ����
		SetScale(Vec2(207.f, 122.f));
		GetCollider()->SetScale(GetScale());
		//�ʿ�� �浹ü ������ �߰�
		GetCollider()->SetOffsetScale(Vec2(-100.f, -60.f));
		GetCollider()->SetOffsetPos(Vec2(0.f, 30.f));
	

		//�ִϸ��̼� ���
		GetAnimator()->LoadAnimation(L"animation\\mapTile\\Anim_Obs2_Idle.anim");
		GetAnimator()->Play(L"anim_Obs2_Idle", true);
	}
		break;
	case OBS_TYPE::ANIM_3:
	{
		//�ִϸ����� ����
		CreateAnimator();

		//�ִϸ��̼� ��Ʋ�� �ҷ�����
		CTexture* pAnimTex = CResourceManager::GetInst()->LoadTexture(L"anim_obs3_Atlas", L"texture\\map\\mapTile\\Anim_Obs\\anim_obs3.png");
		SetTexture(pAnimTex);

		//�浹ü ũ�� ����
		SetScale(Vec2(277.f, 247.f));
		GetCollider()->SetScale(GetScale());
		//�ʿ�� �浹ü ������ �߰�
		GetCollider()->SetOffsetScale(Vec2(-130.f, -110.f));
		GetCollider()->SetOffsetPos(Vec2(10.f, 55.f));

		//�ִϸ��̼� ���
		GetAnimator()->LoadAnimation(L"animation\\mapTile\\Anim_Obs3_Idle.anim");
		GetAnimator()->Play(L"anim_Obs3_Idle", true);

	}
		break;
	case OBS_TYPE::ANIM_4:
	{
		//�ִϸ����� ����
		CreateAnimator();

		//�ִϸ��̼� ��Ʋ�� �ҷ�����
		CTexture* pAnimTex = CResourceManager::GetInst()->LoadTexture(L"anim_obs4_Atlas", L"texture\\map\\mapTile\\Anim_Obs\\anim_obs4.png");
		SetTexture(pAnimTex);

		//�浹ü ũ�� ����
		SetScale(Vec2(91.f, 93.f));
		GetCollider()->SetScale(GetScale());
		//�ʿ�� �浹ü ������ �߰�
		GetCollider()->SetOffsetScale(Vec2(-20.f, -20.f));
		
		//�ִϸ��̼� ���
		GetAnimator()->LoadAnimation(L"animation\\mapTile\\Anim_Obs4_Idle.anim");
		GetAnimator()->Play(L"anim_Obs4_Idle", true);
	}
		break;
	case OBS_TYPE::ANIM_5:
	{
		//�ִϸ����� ����
		CreateAnimator();

		//�ִϸ��̼� ��Ʋ�� �ҷ�����
		CTexture* pAnimTex = CResourceManager::GetInst()->LoadTexture(L"anim_obs5_Atlas", L"texture\\map\\mapTile\\Anim_Obs\\anim_obs5.png");
		SetTexture(pAnimTex);

		//�浹ü ũ�� ����
		SetScale(Vec2(230.f, 201.f));
		GetCollider()->SetScale(GetScale());
		//�ʿ�� �浹ü ������ �߰�
		GetCollider()->SetOffsetScale(Vec2(-130.f, -20.f));
		GetCollider()->SetOffsetPos(Vec2(0.f, 10.f));


		//�ִϸ��̼� ���
		GetAnimator()->LoadAnimation(L"animation\\mapTile\\Anim_Obs5_Idle.anim");
		GetAnimator()->Play(L"anim_Obs5_Idle", true);
	}
		break;
	case OBS_TYPE::ANIM_6:
	{
		//�ִϸ����� ����
		CreateAnimator();

		//�ִϸ��̼� ��Ʋ�� �ҷ�����
		CTexture* pAnimTex = CResourceManager::GetInst()->LoadTexture(L"anim_obs6_Atlas", L"texture\\map\\mapTile\\Anim_Obs\\anim_obs6.png");
		SetTexture(pAnimTex);

		//�浹ü ũ�� ����
		SetScale(Vec2(202.f, 199.f));
		GetCollider()->SetScale(GetScale());
		//�ʿ�� �浹ü ������ �߰�
		GetCollider()->SetOffsetScale(Vec2(-100.f, -20.f));
		GetCollider()->SetOffsetPos(Vec2(0.f, 10.f));

		//�ִϸ��̼� ���
		GetAnimator()->LoadAnimation(L"animation\\mapTile\\Anim_Obs6_Idle.anim");
		GetAnimator()->Play(L"anim_Obs6_Idle", true);
	}
		break;
	case OBS_TYPE::ANIM_7: 
	{
		//�ִϸ����� ����
		CreateAnimator();

		//�ִϸ��̼� ��Ʋ�� �ҷ�����
		CTexture* pAnimTex = CResourceManager::GetInst()->LoadTexture(L"anim_obs78_Atlas", L"texture\\map\\mapTile\\Anim_Obs\\anim_obs78.png");
		SetTexture(pAnimTex);

		//�浹ü ũ�� ����
		SetScale(Vec2(177.f, 206.f));
		GetCollider()->SetScale(GetScale());
		//�ʿ�� �浹ü ������ �߰�
		GetCollider()->SetOffsetScale(Vec2(-100.f, -20.f));
		GetCollider()->SetOffsetPos(Vec2(0.f, 10.f));


		//�ִϸ��̼� ���
		GetAnimator()->LoadAnimation(L"animation\\mapTile\\Anim_Obs7_Idle.anim");
		GetAnimator()->Play(L"anim_Obs7_Idle", true);

	}
		break;
	case OBS_TYPE::ANIM_8:
	{
		//�ִϸ����� ����
		CreateAnimator();

		//�ִϸ��̼� ��Ʋ�� �ҷ�����
		CTexture* pAnimTex = CResourceManager::GetInst()->LoadTexture(L"anim_obs78_Atlas", L"texture\\map\\mapTile\\Anim_Obs\\anim_obs78.png");
		SetTexture(pAnimTex);

		//�浹ü ũ�� ����
		SetScale(Vec2(177.f, 206.f));
		GetCollider()->SetScale(GetScale());
		//�ʿ�� �浹ü ������ �߰�
		GetCollider()->SetOffsetScale(Vec2(-100.f, -20.f));
		GetCollider()->SetOffsetPos(Vec2(0.f, 10.f));

		//�ִϸ��̼� ���
		GetAnimator()->LoadAnimation(L"animation\\mapTile\\Anim_Obs8_Idle.anim");
		GetAnimator()->Play(L"anim_Obs8_Idle", true);
	}
		break;
	default:
		break;
	}
}

void CObstacle::tick()
{
	if (m_bParabola)
	{
		Parabola();
	}

	CMapTile::tick();
}

void CObstacle::render(HDC _dc)
{
	if (m_eObsType == OBS_TYPE::FIXED)
	{
		Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());
		Vec2 vScale = GetScale();

		//AlphaBlend ����
		BLENDFUNCTION tBlend = {};

		tBlend.AlphaFormat = AC_SRC_ALPHA;
		tBlend.BlendFlags = 0;
		tBlend.BlendOp = AC_SRC_OVER;
		tBlend.SourceConstantAlpha = int(255.f);

		AlphaBlend(_dc
			, (int)(vPos.x - m_pTexture->GetWidth()/2)
			, (int)(vPos.y - m_pTexture->GetHeight() / 2)
			, m_pTexture->GetWidth()
			, m_pTexture->GetHeight()
			, m_pTexture->GetDC()
			, 0, 0
			, m_pTexture->GetWidth()
			, m_pTexture->GetHeight()
			, tBlend);
	}

	CObj::render(_dc);
}

void CObstacle::Parabola()
{
	//���� �ð��� �޴´�
	m_iLifeTime += DT;

	Vec2 pos = GetPos();				//offset 3000 fangle 30 v0 1100 gravity 1000
		
	float fOffset = 3000;
	float fAngle = 30;
	fAngle = fAngle - (fOffset * DT);			// angle�� 0���� ���� 

	static const float v0 = 1100.0f;				// ������ ��(Velocity)
	static const float gravity = 1000.f;			// �߷�

	float radian = fAngle * PI / 180;			// ������ �������� ��ȯ
	float sin = sinf(radian);                // sin�� �̸� ���ϱ�
	float cos = cosf(radian);                // cos�� �̸� ���ϱ�

	// ������ �
	pos.y = pos.y - ((v0 * sin * DT) - ((1.0f / 2.0f) * gravity * DT * DT));
	pos.x = pos.x + (v0 * cos * DT);
	SetPos(pos);

	if (m_iLifeTime >= 1.f)
	{
		m_bParabola = false;
		this->SetDead();
	}
}

void CObstacle::BeginOverlap(CCollider* _pOther)
{
	//���̳���ĳ��Ʈ�� �̿��Ͽ� �浹�� ������Ʈ�� player Ŭ�������� Ȯ���� �� �ִ�
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetOwner());
	if (pPlayer == nullptr)
		return;

	if (pPlayer->IsBooster() || pPlayer->IsBigger() || (pPlayer->GetName() == L"SnowSugarCookie"&& pPlayer->IsOnSkill()))
	{
		//ȸ���� �Ѵ�

		//ȭ�� ������ ���ư���
		m_bParabola=true;

		pPlayer->GetInvincibleCrashSFX()->Play();
	}

	else if (pPlayer->IsOnShield())
	{
		pPlayer->SetCrashed(true);
		pPlayer->SetOnShield(false);
	}

	else
	{
		pPlayer->SetCrashed(true);
		pPlayer->SetHP(pPlayer->GetHP() - 50.f);
	}

}

void CObstacle::OnOverlap(CCollider* _pOther)
{
}

void CObstacle::EndOverlap(CCollider* _pOther)
{
}
