#include "pch.h"
#include "CStage1.h"
#include "CCamera.h"
#include "CEngine.h"
#include "CPlayer.h"
#include "CUI.h"
#include "CHpBarUI.h"
#include "CPet.h"
#include "CResourceManager.h"
#include "CBackground.h"
#include "CGingerBraveCookie.h"
#include "CMilkCookie.h"
#include "CPetMilkBottle.h"
#include "CPetChocoDrop.h"
#include "CJumpBtn.h"
#include "CSlideBtn.h"
#include "CTextUI.h"
#include "CPanelUI.h"
#include "CKeyManager.h"
#include "CResultLevel.h"
#include "CLevelManager.h"
#include "CSound.h"
#include "CTimeManager.h"
#include "CBonusBar.h"
#include "CCookieSkillBar.h"
#include "CFairyCookie.h"
#include "CPetFlowerPod.h"
#include "CSnowSugarCookie.h"
#include "CPetSnowGlobe.h"

CStage1::CStage1()
	: m_eCookieType(SELECT_COOKIE::NONE)
	, m_ePetType(SELECT_PET::NONE)
	, m_bMatchBonus(false)
{
	SetColliderRender(false);
}

CStage1::~CStage1()
{
	
}

void CStage1::init()
{
	//맵 Load
	LoadMap(L"map\\Tuto5.map");

	//Background 생성
	CObj* pBackground = new CBackground;
	static_cast<CBackground*>(pBackground)->init();
	pBackground->SetPos(Vec2(0.f, 0.f));
	AddObject(pBackground, LAYER::BACKGROUND);

	//플레이어 생성 및 초기셋팅
	vector<CObj*> vecPlayer = GetLayer(LAYER::PLAYER);
	if (!vecPlayer.empty())
	{
 		SetPlayer(static_cast<CPlayer*>(vecPlayer.front()));
	}
	
	if (!GetPlayer())
	{
		CPlayer* pPlayer = nullptr;
		switch (m_eCookieType)
		{
		case SELECT_COOKIE::GINGERBRAVE:
		{
			pPlayer = new CGingerBraveCookie;
		}
			break;
		case SELECT_COOKIE::MILK:
		{
			pPlayer = new CMilkCookie;
		}
			break;
		case SELECT_COOKIE::FAIRY:
		{
			pPlayer = new CFairyCookie;
		}
		break;
		case SELECT_COOKIE::SNOWSUGAR:
		{
			pPlayer = new CSnowSugarCookie;
		}
		break;

		default:
			break;
		}

		pPlayer->SetPos(Vec2(200.f, 500.f));
		//플레이어 오브젝트 배열에 넣어주기
		AddObject(pPlayer, LAYER::PLAYER);

		SetPlayer(pPlayer);
	}

	vector<CObj*> vecPet = GetLayer(LAYER::PET);
	if (!vecPet.empty())
	{
		SetPet(static_cast<CPet*>(vecPet.front()));
	}

	if (!GetPet())
	{
		CPet* pPet = nullptr;

		switch (m_ePetType)
		{
		case SELECT_PET::CHOCODROP:
		{
			pPet = new CPetChocoDrop;
		}
			break;
		case SELECT_PET::MILKBOTTLE:
		{
			pPet = new CPetMilkBottle;
		}
			break;
		case SELECT_PET::FLOWERPOD:
		{
			pPet = new CPetFlowerPod;
		}
		break;
		case SELECT_PET::SNOWGLOBE:
		{
			pPet = new CPetSnowGlobe;
		}
		break;
		default:
			break;
		}

		AddObject(pPet, LAYER::PET);
		SetPet(pPet);
	}

	//Match Bonus 상태 체크
	if (m_bMatchBonus)
	{
		GetPlayer()->SetMatchBonus(true);

		switch (m_eCookieType)
		{
		case SELECT_COOKIE::GINGERBRAVE:
		{
			static_cast<CGingerBraveCookie*>(GetPlayer())->init();
		}
			break;
		case SELECT_COOKIE::MILK:
		{
			static_cast<CMilkCookie*>(GetPlayer())->init();
		}
			break;
		case SELECT_COOKIE::FAIRY:
		{
			static_cast<CFairyCookie*>(GetPlayer())->init();
		}
			break;
		case SELECT_COOKIE::SNOWSUGAR:
		{
			static_cast<CSnowSugarCookie*>(GetPlayer())->init();
		}
		break;
		default:
			break;
		}
	}


	//=========UI============
 	initUI();
	
	//플레이어 위치 정보
	Vec2 vPos = GetPlayer()->LoadPos();
	GetPlayer()->SetPos(Vec2(vPos.x, 0.f));

	ResetCollision();

	//BGM 재생
	m_pBGM = CResourceManager::GetInst()->LoadSound(L"BGM_Stage1", L"sound\\BGM\\BGM_Stage1.wav");
	m_pBGM->PlayToBGM(true);
}

void CStage1::tick()
{
	//카메라가 보고 있는 위치를 해상도의 중심으로 설정(좌상단의 좌표가 0,0이 될 수 있게)
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	Vec2 vLook = CCamera::GetInst()->GetLook();
	Vec2 vOffset = Vec2(330.f, -280.f);
	CCamera::GetInst()->SetLook(Vec2((GetPlayer()->GetPos()).x + vOffset.x, 300.f));

	//Text UI 갱신
	m_pCoinText->SetNumber(GetPlayer()->GetCoin());
	m_pScoreText->SetNumber(GetPlayer()->GetScore());

	if (m_pSkillBar != nullptr)
	{
		Vec2 vPos = GetPlayer()->GetPos() + Vec2(-54.f, -180.f);
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
		m_pSkillBar->SetPos(vPos);
	}

	if (IsTap(KEY::LCTRL))
	{
		if (GetColliderRender())
		{
			SetColliderRender(false);
		}
		else
		{
			SetColliderRender(true);
		}
	}

	if (IsTap(KEY::ESC))
	{
		if (CLevelManager::GetInst()->GetCurLevel()->GetPause())
			return;

		Pause();
	}


	CLevel::tick();
}

void CStage1::Enter()
{
	init();
}

void CStage1::Exit()
{
	//BGM Stop
	m_pBGM->Stop(false);

	if (!GetPlayer()->GetPlayerDead())
	{
		GetLayer(LAYER::PLAYER).clear();
		GetLayer(LAYER::PET).clear();
		GetLayer(LAYER::UI).clear();
		GetLayer(LAYER::PLAYER_SKILL).clear();

		LevelDestroy();
	}

	else if (GetPlayer()->GetPlayerDead())
	{
		CResultLevel* pLevel = dynamic_cast<CResultLevel*>(CLevelManager::GetInst()->GetLevel((UINT)LEVEL_TYPE::RESULT));
		pLevel->SetScore(GetPlayer()->GetScore());
		pLevel->SetCoin(GetPlayer()->GetCoin());
		DeleteAllObject();
	}
}

void CStage1::CreateTextUI()
{
	//Score TextUI
	m_pScoreText = new CTextUI;
	m_pScoreText->SetNumber(GetPlayer()->GetScore());
	m_pScoreText->SetFontSize(40);
	m_pScoreText->SetFontAlign(FONT_ALIGN::CENTER);
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	m_pScoreText->SetPos(vResolution / 2.f - Vec2(0.f, 250.f));
	AddObject(m_pScoreText, LAYER::UI);

	//Score 아이콘
	CPanelUI* pScoreIcon = new CPanelUI;
	CTexture* pTex = CResourceManager::GetInst()->LoadTexture(L"CurScore", L"texture\\UI\\CurScore.png");
	pScoreIcon->SetIdleTex(pTex);
	Vec2 vPos = Vec2(vResolution.x / 2.f - 53.f, 75.f);
	pScoreIcon->SetPos(vPos);
	AddObject(pScoreIcon, LAYER::UI);

	//Coin TextUI
	m_pCoinText = new CTextUI;
	m_pCoinText->SetNumber(GetPlayer()->GetCoin());
	m_pCoinText->SetFontSize(40);
	m_pCoinText->SetFontAlign(FONT_ALIGN::LEFT);
	m_pCoinText->SetPos(Vec2(50.f, 95.f));
	AddObject(m_pCoinText, LAYER::UI);

	//Coin 아이콘
	CPanelUI* pCoinIcon = new CPanelUI;
	pTex = CResourceManager::GetInst()->LoadTexture(L"CoinIcon", L"texture\\UI\\CoinIcon.png");
	pCoinIcon->SetIdleTex(pTex);
	vPos = Vec2(10.f, 96.f);
	pCoinIcon->SetPos(vPos);
	AddObject(pCoinIcon, LAYER::UI);
}


void CStage1::initUI()
{
	if (GetLayer(LAYER::UI).empty())
	{
		//HPBar
		CUI* pHpBar = new CHpBarUI;
		AddObject(pHpBar, LAYER::UI);
		//BonusBar
		CUI* pBonusBar = new CBonusBar;
		AddObject(pBonusBar, LAYER::UI);
		//CookieSkillBar
		if (!GetPlayer()->GetSkillCoolTime() == 0)
		{
			m_pSkillBar = new CCookieSkillBar;
			AddObject(m_pSkillBar, LAYER::UI);
		}
		//JumpBtn
		CUI* pJumpBtn = new CJumpBtn;
		AddObject(pJumpBtn, LAYER::UI);
		//SlideBtn
		CUI* pSlideBtn = new CSlideBtn;
		AddObject(pSlideBtn, LAYER::UI);

		//점수, 코인 텍스트 UI 생성
		CreateTextUI();

		//일시정지 버튼 생성
		m_pPauseBtn = new CButtonUI;
		CTexture* pPauseBtnTex = CResourceManager::GetInst()->LoadTexture(L"PauseBtn", L"texture\\UI\\PauseBtn.png");
		m_pPauseBtn->SetIdleTex(pPauseBtnTex);
		Vec2 vResolution = CEngine::GetInst()->GetResolution();
		m_pPauseBtn->SetPos(Vec2(vResolution.x - 70.f, 15.f));
		m_pPauseBtn->SetDelegate(this, (DELEGATE)&CLevel::Pause);
		AddObject(m_pPauseBtn, LAYER::UI);
	}
}


