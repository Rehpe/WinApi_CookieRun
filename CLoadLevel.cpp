#include "pch.h"
#include "CLoadLevel.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CPanelUI.h"
#include "CEngine.h"
#include "CTimeManager.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CStage1.h"
#include "CSound.h"

CLoadLevel::CLoadLevel()
	: m_bMatchBonus(false)
	, m_eCookieType(SELECT_COOKIE::NONE)
	, m_ePetType(SELECT_PET::NONE)
	, m_pBackground(nullptr)
{
}

CLoadLevel::~CLoadLevel()
{
}

void CLoadLevel::init()
{
	m_pBackground = new CPanelUI;

	CTexture* pTex = CResourceManager::GetInst()->LoadTexture(L"LoadScreen", L"texture\\UI\\LoadScreen.png");
	m_pBackground->SetIdleTex(pTex);

	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	m_pBackground->SetPos(Vec2(0.f,0.f));

	AddObject(m_pBackground, LAYER::UI);
	
	LoadCookieTex();

	m_pCookieImg = new CPanelUI;
	m_pCookieImg->SetPos(vResolution / 2.f + Vec2(-150.f,-140.f));
	m_pCookieImg->SetIdleTex(m_vecCookieTex[(int)m_eCookieType - 1]);
	m_pBackground->AddChildUI(m_pCookieImg);

	//BGM Àç»ý
	m_pBGM = CResourceManager::GetInst()->LoadSound(L"BGM_Load", L"sound\\BGM\\BGM_Load.wav");
	m_pBGM->PlayToBGM();
}

void CLoadLevel::tick()
{
	static float fAccTime = 0;

	fAccTime += DT;	

	CLevel::tick();

	//2ÃÊ ÈÄ¿¡ ·¹º§ Ã¼ÀÎÁö
	if (fAccTime >= 2.f)
	{
		fAccTime = 0;
		ChangeLevel(LEVEL_TYPE::STAGE_01);
	}
}

void CLoadLevel::Enter()
{
	init();
}

void CLoadLevel::Exit()
{
	CStage1* pLevel = dynamic_cast<CStage1*>(CLevelManager::GetInst()->GetLevel((UINT)LEVEL_TYPE::STAGE_01));

	pLevel->SetCookieType(m_eCookieType);
	pLevel->SetPetType(m_ePetType);
	pLevel->SetMatchBonus(m_bMatchBonus);

	//m_pBGM->Stop();

	DeleteAllObject();
}

void CLoadLevel::LoadCookieTex()
{
	//¿ë°¨ÇÑ ÄíÅ°
	CTexture* pTex = CResourceManager::GetInst()->LoadTexture(L"pLoad_GingerBrave", L"texture\\preview\\pLoad_GingerBrave.png");
	m_vecCookieTex.push_back(pTex);

	//¿ìÀ¯¸À ÄíÅ°
	pTex = CResourceManager::GetInst()->LoadTexture(L"pLoad_Milk", L"texture\\preview\\pLoad_Milk.png");
	m_vecCookieTex.push_back(pTex);

	//¿äÁ¤¸À ÄíÅ°
	pTex = CResourceManager::GetInst()->LoadTexture(L"pLoad_Fairy", L"texture\\preview\\pLoad_Fairy.png");
	m_vecCookieTex.push_back(pTex);

	//´«¼³ÅÁ¸À ÄíÅ°
	pTex = CResourceManager::GetInst()->LoadTexture(L"pLoad_SnowSugar", L"texture\\preview\\pLoad_SnowSugar.png");
	m_vecCookieTex.push_back(pTex);

}
