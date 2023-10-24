#include "pch.h"
#include "CResultLevel.h"
#include "CResourceManager.h"
#include "CPanelUI.h"
#include "CButtonUI.h"
#include "CEngine.h"
#include "CTextUI.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CBonusStage.h"
#include "CStage1.h"
#include "CSound.h"

void ReturntoSelectLevel()
{
	ChangeLevel(LEVEL_TYPE::SELECT);
}

CResultLevel::CResultLevel()
	: m_pBackground(nullptr)
	, m_pOKBtn(nullptr)
	, m_iScore(0)
	, m_iCoin(0)
	, m_pCoinText(nullptr)
	, m_pScoreText(nullptr)
{
}

CResultLevel::~CResultLevel()
{
}

void CResultLevel::init()
{
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	//배경 패널 생성 후 텍스처 대입
	CTexture* pTex = CResourceManager::GetInst()->LoadTexture(L"ResultImg", L"texture\\UI\\ResultImg.png");

	m_pBackground = new CPanelUI;
	m_pBackground->SetIdleTex(pTex);
	m_pBackground->SetPos(Vec2(0.f, 0.f));
	

	//확인 버튼
	CTexture* pOKBtnTex = CResourceManager::GetInst()->LoadTexture(L"Result_OkBtn", L"texture\\UI\\Result_OkBtn.png");

	m_pOKBtn = new CButtonUI;
	m_pOKBtn->SetIdleTex(pOKBtnTex);
	m_pOKBtn->SetPos(vResolution / 2.f + Vec2(-100.f, 210.f));
	m_pOKBtn->SetCallBack(&ReturntoSelectLevel);
	m_pBackground->AddChildUI(m_pOKBtn);

	//점수 Text UI
	m_pScoreText = new CTextUI;
	m_pScoreText->SetNumber(m_iScore);
	m_pScoreText->SetFontSize(80);
	m_pScoreText->SetFontAlign(FONT_ALIGN::CENTER);
	m_pScoreText->SetPos(vResolution / 2.f + Vec2(0.f, -70.f));
	m_pBackground->AddChildUI(m_pScoreText);

	//코인 Text UI
	m_pCoinText = new CTextUI;
	m_pCoinText->SetNumber(m_iCoin);
	m_pCoinText->SetFontSize(80);
	m_pCoinText->SetFontAlign(FONT_ALIGN::CENTER);
	m_pCoinText->SetPos(vResolution / 2.f + Vec2(00.f, 115.f));
	m_pBackground->AddChildUI(m_pCoinText);

	AddObject(m_pBackground, LAYER::UI);

	//BGM 재생
	m_pBGM = CResourceManager::GetInst()->LoadSound(L"BGM_Result", L"sound\\BGM\\BGM_Result.wav");
	m_pBGM->PlayToBGM();
}

void CResultLevel::tick()
{
	CLevel::tick();

}

void CResultLevel::Enter()
{
	init();
}

void CResultLevel::Exit()
{
	//지금껏 거쳐온 모든 레벨의 Object를 삭제한다

	CStage1* pLevel = dynamic_cast<CStage1*>(CLevelManager::GetInst()->GetLevel((UINT)LEVEL_TYPE::STAGE_01));
	pLevel->SetPlayer(nullptr);
	pLevel->SetPet(nullptr);
	pLevel->DeleteAllObject();

	CBonusStage* pBonusLevel = dynamic_cast<CBonusStage*>(CLevelManager::GetInst()->GetLevel((UINT)LEVEL_TYPE::BONUS));
	pBonusLevel->DeleteAllObject();

	m_pBGM->Stop();

	DeleteAllObject();
}
