#include "pch.h"
#include "CIntroLevel.h"
#include "CPanelUI.h"
#include "CButtonUI.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CEngine.h"
#include "CSoundManager.h"
#include "CSound.h"

void ChangeSelectLevel()
{
	ChangeLevel(LEVEL_TYPE::SELECT);
}

void ChangeEditLevel()
{
	ChangeLevel(LEVEL_TYPE::EDITOR);
}

CIntroLevel::CIntroLevel()
	:m_pPanelUI(nullptr)
	,m_pPlayBtn(nullptr)
	,m_pEditBtn(nullptr)
{
}

CIntroLevel::~CIntroLevel()
{
}

void CIntroLevel::init()
{
	//배경, 버튼 텍스쳐 불러오기
	CTexture* pIntroPanelTex = CResourceManager::GetInst()->LoadTexture(L"IntroBackground", L"texture\\IntroBackground.png");
	CTexture* pPlayBtnTex = CResourceManager::GetInst()->LoadTexture(L"IntroPlayBtn", L"texture\\UI\\Intro_GamePlayBtn.png");
	CTexture* pEditBtnTex = CResourceManager::GetInst()->LoadTexture(L"IntroEditBtn", L"texture\\UI\\Intro_MapEditorBtn.png");

	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	//배경 설정
	m_pPanelUI = new CPanelUI;
	m_pPanelUI->SetIdleTex(pIntroPanelTex);
	m_pPanelUI->SetPos(Vec2(0.f, 0.f));

	//버튼 설정
	//플레이 버튼
	m_pPlayBtn = new CButtonUI;
	m_pPlayBtn->SetIdleTex(pPlayBtnTex);
	m_pPlayBtn->SetPos(vResolution / 2.f - Vec2(100.f, -172.f));

	//에디터 버튼
	m_pEditBtn = new CButtonUI;
	m_pEditBtn->SetIdleTex(pEditBtnTex);
	m_pEditBtn->SetPos(vResolution / 2.f - Vec2(100.f, -249.f));

	//각 버튼에 Callback 부여
	m_pPlayBtn->SetCallBack(&ChangeSelectLevel);
	m_pEditBtn->SetCallBack(&ChangeEditLevel);

	//버튼의 부모 설정
	m_pPanelUI->AddChildUI(m_pPlayBtn);
	m_pPanelUI->AddChildUI(m_pEditBtn);

	//Panel UI 레이어에 등록
	AddObject(m_pPanelUI, LAYER::UI);

	//BGM 재생
	m_pBGM = CResourceManager::GetInst()->LoadSound(L"IntroBGM", L"sound\\BGM\\BGM_Intro.wav");
	m_pBGM->PlayToBGM(true);
}


void CIntroLevel::tick()
{
	CLevel::tick();
}

void CIntroLevel::Enter()
{
	init();
}

void CIntroLevel::Exit()
{
	m_pBGM->Stop();
	DeleteAllObject();
}
