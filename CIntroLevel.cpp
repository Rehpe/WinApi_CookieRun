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
	//���, ��ư �ؽ��� �ҷ�����
	CTexture* pIntroPanelTex = CResourceManager::GetInst()->LoadTexture(L"IntroBackground", L"texture\\IntroBackground.png");
	CTexture* pPlayBtnTex = CResourceManager::GetInst()->LoadTexture(L"IntroPlayBtn", L"texture\\UI\\Intro_GamePlayBtn.png");
	CTexture* pEditBtnTex = CResourceManager::GetInst()->LoadTexture(L"IntroEditBtn", L"texture\\UI\\Intro_MapEditorBtn.png");

	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	//��� ����
	m_pPanelUI = new CPanelUI;
	m_pPanelUI->SetIdleTex(pIntroPanelTex);
	m_pPanelUI->SetPos(Vec2(0.f, 0.f));

	//��ư ����
	//�÷��� ��ư
	m_pPlayBtn = new CButtonUI;
	m_pPlayBtn->SetIdleTex(pPlayBtnTex);
	m_pPlayBtn->SetPos(vResolution / 2.f - Vec2(100.f, -172.f));

	//������ ��ư
	m_pEditBtn = new CButtonUI;
	m_pEditBtn->SetIdleTex(pEditBtnTex);
	m_pEditBtn->SetPos(vResolution / 2.f - Vec2(100.f, -249.f));

	//�� ��ư�� Callback �ο�
	m_pPlayBtn->SetCallBack(&ChangeSelectLevel);
	m_pEditBtn->SetCallBack(&ChangeEditLevel);

	//��ư�� �θ� ����
	m_pPanelUI->AddChildUI(m_pPlayBtn);
	m_pPanelUI->AddChildUI(m_pEditBtn);

	//Panel UI ���̾ ���
	AddObject(m_pPanelUI, LAYER::UI);

	//BGM ���
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
