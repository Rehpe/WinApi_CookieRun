#include "pch.h"
#include "CSelectLevel.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CEngine.h"
#include "CPanelUI.h"
#include "CButtonUI.h"
#include "CGingerBraveCookie.h"
#include "CMilkCookie.h"
#include "CFairyCookie.h"
#include "CPetChocoDrop.h"
#include "CPetMilkBottle.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CSelectBtn.h"
#include "CLevel.h"
#include "CLevelManager.h"
#include "CLoadLevel.h"
#include "CCamera.h"
#include "CSound.h"

void ReturnToIntroLevel()
{
	ChangeLevel(LEVEL_TYPE::INTRO);
}

CSelectLevel::CSelectLevel()
	: m_pPanelUI(nullptr)
	, m_pPlayBtn(nullptr)
	, m_pCookieAnim(nullptr)
	, m_pPetAnim(nullptr)
	, m_vecCookieInfo{}
	, m_vecPetInfo{}
	, m_eCookieType(SELECT_COOKIE::NONE)
	, m_ePetType(SELECT_PET::NONE)
{
}

CSelectLevel::~CSelectLevel()
{
}

void CSelectLevel::init()
{
	CCamera::GetInst()->SetLook(Vec2(0.f, 0.f));

	//���, ��ư �ؽ��� �ҷ�����
	CTexture* pSelectPanelTex = CResourceManager::GetInst()->LoadTexture(L"SelectPanel", L"texture\\UI\\Select_Panel.png");
	CTexture* pPlayBtnTex = CResourceManager::GetInst()->LoadTexture(L"SelectPlayBtn", L"texture\\UI\\Select_PlayBtn.png");

	//����â ��� ����
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	m_pPanelUI = new CPanelUI;
	m_pPanelUI->SetIdleTex(pSelectPanelTex);
	m_pPanelUI->SetPos(Vec2(0.f, 0.f));

	//�ִϸ��̼� ����� ���� �г� ������ �ִϸ����� ����, ����� �ִϸ��̼� �ҷ�����
	m_pCookieAnim = new CPanelUI;
	m_pCookieAnim->CreateAnimator();
	m_pCookieAnim->SetPos(Vec2(450.f, 100.f));
	m_pCookieAnim->GetAnimator()->LoadAnimation(L"animation\\Preview\\GingerBrave_Run.anim");
	m_pCookieAnim->GetAnimator()->LoadAnimation(L"animation\\Preview\\MilkCookie_Run.anim");
	m_pCookieAnim->GetAnimator()->LoadAnimation(L"animation\\Preview\\FairyCookie_Run.anim");
	m_pCookieAnim->GetAnimator()->LoadAnimation(L"animation\\Preview\\SnowSugarCookie_Run.anim");


	m_pPetAnim = new CPanelUI;
	m_pPetAnim->CreateAnimator();
	m_pPetAnim->SetPos(Vec2(350.f, 50.f));
	m_pPetAnim->GetAnimator()->LoadAnimation(L"animation\\Preview\\ChocoDrop_Idle.anim");
	m_pPetAnim->GetAnimator()->LoadAnimation(L"animation\\Preview\\MilkBottle_Idle.anim");
	m_pPetAnim->GetAnimator()->LoadAnimation(L"animation\\Preview\\FlowerPod_Idle.anim");
	m_pPetAnim->GetAnimator()->LoadAnimation(L"animation\\Preview\\SnowGlobe_Idle.anim");


	m_pPanelUI->AddChildUI(m_pCookieAnim);
	m_pPanelUI->AddChildUI(m_pPetAnim);

	//��ư ����
	//�÷��� ��ư
	m_pPlayBtn = new CButtonUI;
	m_pPlayBtn->SetIdleTex(pPlayBtnTex);
	m_pPlayBtn->SetPos(vResolution / 2.f + Vec2(265.f, 213.f));
	m_pPlayBtn->SetDelegate(this,(DELEGATE) &CSelectLevel::ChangeLoadLevel);
	m_pPanelUI->AddChildUI(m_pPlayBtn);

	//�ڷΰ��� ��ư

	m_pBackBtn = new CButtonUI;
	CTexture* pBackBtnTex = CResourceManager::GetInst()->LoadTexture(L"BackBtn", L"texture\\UI\\BackBtn.png");
	m_pBackBtn->SetIdleTex(pBackBtnTex);
	m_pBackBtn->SetPos(Vec2(5.f, 5.f));
	m_pBackBtn->SetCallBack(&ReturnToIntroLevel);
	m_pPanelUI->AddChildUI(m_pBackBtn);

	//��Ű, �� ���� ���� ä���
	LoadCookieInfo();
	LoadPetInfo();

	//��Ű, �� ��ư �����
	CreateCookieBtn();
	CreatePetBtn();

	//��Ű, �� ��ũ���� �����
	LoadCookieDesc();
	LoadPetDesc();

	//MatchBonus �����
	m_pMatchBonus = new CPanelUI;
	m_pMatchBonus->SetIdleTex(nullptr);
	m_pMatchBonus->SetPos(Vec2(940.f, 130.f));
	m_pPanelUI->AddChildUI(m_pMatchBonus);

	//Panel UI ���̾ ���
	AddObject(m_pPanelUI, LAYER::UI);

	//����Ʈ�� �밨����Ű, ���ڹ�� ���õǾ�����
	m_eCookieType = SELECT_COOKIE::GINGERBRAVE;
	m_ePetType = SELECT_PET::CHOCODROP;

	m_pCookieAnim->GetAnimator()->Play(L"GingerBrave", true);
	m_pPetAnim->GetAnimator()->Play(L"ChocoDrop", true);

	//BGM ���
	m_pBGM = CResourceManager::GetInst()->LoadSound(L"BGM_Select", L"sound\\BGM\\BGM_Select.wav");
	m_pBGM->PlayToBGM(true);

}

void CSelectLevel::tick()
{
	//������ ��Ű ����â ����
	m_pCookieDesc->SetIdleTex(m_vecCookieDesc[(int)m_eCookieType]);
	
	//������ �� ����â ����
	m_pPetDesc->SetIdleTex(m_vecPetDesc[(int)m_ePetType]);

	//������ ��Ű�� ���� ¦���̸� Matchbonus Ȱ��ȭ
	if ((int)m_ePetType && (int)m_eCookieType && (int)m_eCookieType == (int)m_ePetType)
	{
		m_bMatchBonus = true;
		CTexture* pMatchTex = CResourceManager::GetInst()->LoadTexture(L"Select_MatchBonus", L"texture\\UI\\Select_MatchBonus.png");
		m_pMatchBonus->SetIdleTex(pMatchTex);
	}
	else
	{
		m_bMatchBonus = false;
		m_pMatchBonus->SetIdleTex(nullptr);
	}

	CLevel::tick();
}

void CSelectLevel::Enter()
{
	init();
}

void CSelectLevel::Exit()
{
	m_pBGM->Stop();
	m_vecCookieInfo.clear();
	m_vecPetInfo.clear();
	DeleteAllObject();
}

void CSelectLevel::LoadCookieInfo()
{
	//�밨�� ��Ű
	tCookieInfo tCookie = {};
	tCookie.strKey = L"GingerBrave_Preview";
	tCookie.strPreviewPath = L"texture\\Preview\\pGingerBrave.png";
	tCookie.eCookieType = SELECT_COOKIE::GINGERBRAVE;
	m_vecCookieInfo.push_back(tCookie);

	//������ ��Ű
	tCookie = {};
	tCookie.strKey = L"Milk_Preview";
	tCookie.strPreviewPath = L"texture\\Preview\\pMilk.png";
	tCookie.eCookieType = SELECT_COOKIE::MILK;
	m_vecCookieInfo.push_back(tCookie);

	//������ ��Ű
	tCookie = {};
	tCookie.strKey = L"Fairy_Preview";
	tCookie.strPreviewPath = L"texture\\Preview\\pFairy.png";
	tCookie.eCookieType = SELECT_COOKIE::FAIRY;
	m_vecCookieInfo.push_back(tCookie);

	//�������� ��Ű
	tCookie = {};
	tCookie.strKey = L"SnowSugar_Preview";
	tCookie.strPreviewPath = L"texture\\Preview\\pSnowSugar.png";
	tCookie.eCookieType = SELECT_COOKIE::SNOWSUGAR;
	m_vecCookieInfo.push_back(tCookie);
}

void CSelectLevel::LoadPetInfo()
{
	//���ڹ��(�밨�� ��Ű)
	tPetInfo tPet = {};
	tPet.strKey = L"ChocoDrop_Preview";
	tPet.strPreviewPath = L"texture\\Preview\\pPetChocoDrop.png";
	tPet.ePetType = SELECT_PET::CHOCODROP;
	m_vecPetInfo.push_back(tPet);

	//������Ʋ����(������ ��Ű)
	tPet = {};
	tPet.strKey = L"MilkBottle_Preview";
	tPet.strPreviewPath = L"texture\\Preview\\pPetMilkBottle.png";
	tPet.ePetType = SELECT_PET::MILKBOTTLE;
	m_vecPetInfo.push_back(tPet);

	//�ɺ�����(������ ��Ű)
	tPet = {};
	tPet.strKey = L"FlowerPod_Preview";
	tPet.strPreviewPath = L"texture\\Preview\\pPetFlowerPod.png";
	tPet.ePetType = SELECT_PET::FLOWERPOD;
	m_vecPetInfo.push_back(tPet);

	//����� �۷κ�(�������� ��Ű)
	tPet = {};
	tPet.strKey = L"SnowGlobe_Preview";
	tPet.strPreviewPath = L"texture\\Preview\\pPetSnowGlobe.png";
	tPet.ePetType = SELECT_PET::SNOWGLOBE;
	m_vecPetInfo.push_back(tPet);
}

void CSelectLevel::CreateCookieBtn()
{
	//��Ű���� ��ư �����
	Vec2 vInnerPadding = Vec2(20.f, 10.f);
	Vec2 vLeftTop = Vec2(35.f, 110.f);
	const int BtnSize = 119;

	for (int iRow = 0; iRow < 2; iRow++)
	{
		for (int iCol = 0; iCol < 3; iCol++)
		{
			int iIndexNum = iCol + (3 * iRow);

			CSelectBtn* pButton = new CSelectBtn;
			pButton->SetPos(Vec2(vLeftTop + Vec2((BtnSize * iCol) + vInnerPadding.x * iCol, (BtnSize * iRow) + vInnerPadding.y * iRow)));

			if (m_vecCookieInfo.size() >= iIndexNum + 1)
			{
				CTexture* pPrevieTex = CResourceManager::GetInst()->LoadTexture(m_vecCookieInfo[iIndexNum].strKey, m_vecCookieInfo[iIndexNum].strPreviewPath);
				pButton->SetIdleTex(pPrevieTex);
				pButton->SetCookieType(m_vecCookieInfo[iIndexNum].eCookieType);
				pButton->SetDelegate(pButton, (DELEGATE)&CSelectBtn::selectObject);
			}
			m_pPanelUI->AddChildUI(pButton);
		}
	}
}

void CSelectLevel::CreatePetBtn()
{
	//�꼱�� ��ư �����
	Vec2 vInnerPadding = Vec2(20.f, 10.f);
	Vec2 vLeftTop = Vec2(35.f, 435.f);
	const int BtnSize = 119;

	for (int iRow = 0; iRow < 2; iRow++)
	{
		for (int iCol = 0; iCol < 3; iCol++)
		{
			int iIndexNum = iCol + (3 * iRow);

			CSelectBtn* pButton = new CSelectBtn;
			pButton->SetPos(Vec2(vLeftTop + Vec2((BtnSize * iCol) + vInnerPadding.x * iCol, (BtnSize * iRow) + vInnerPadding.y * iRow)));

			if (m_vecPetInfo.size() >= iIndexNum + 1)
			{
				CTexture* pPrevieTex = CResourceManager::GetInst()->LoadTexture(m_vecPetInfo[iIndexNum].strKey, m_vecPetInfo[iIndexNum].strPreviewPath);
				pButton->SetIdleTex(pPrevieTex);
				pButton->SetPetType(m_vecPetInfo[iIndexNum].ePetType);
				pButton->SetDelegate(pButton, (DELEGATE)&CSelectBtn::selectObject);
			}

			m_pPanelUI->AddChildUI(pButton);
		}
	}
}

void CSelectLevel::LoadCookieDesc()
{
	//��Ű ����â �����
	m_pCookieDesc = new CPanelUI;
	m_pCookieDesc->SetPos(Vec2(478.f, 100.f));

	m_pPanelUI->AddChildUI(m_pCookieDesc);

	//��ũ���� ���Ϳ� �ؽ�ó �ε�
	CTexture* pTex = CResourceManager::GetInst()->LoadTexture(L"DefaultDesc", L"texture\\UI\\Description\\DefaultDesc.png");
	m_vecCookieDesc.push_back(pTex);

	pTex = CResourceManager::GetInst()->LoadTexture(L"GingerBraveDesc", L"texture\\UI\\Description\\GingerBraveDesc.png");
	m_vecCookieDesc.push_back(pTex);

	pTex = CResourceManager::GetInst()->LoadTexture(L"MilkDesc", L"texture\\UI\\Description\\MilkDesc.png");
	m_vecCookieDesc.push_back(pTex);

	pTex = CResourceManager::GetInst()->LoadTexture(L"FairyDesc", L"texture\\UI\\Description\\FairyDesc.png");
	m_vecCookieDesc.push_back(pTex);

	pTex = CResourceManager::GetInst()->LoadTexture(L"SnowSugarDesc", L"texture\\UI\\Description\\SnowSugarDesc.png");
	m_vecCookieDesc.push_back(pTex);
}

void CSelectLevel::LoadPetDesc()
{
	//�� ����â �����
	m_pPetDesc = new CPanelUI;
	m_pPetDesc->SetPos(Vec2(478.f, 425.f));

	m_pPanelUI->AddChildUI(m_pPetDesc);

	//��ũ���� ���Ϳ� �ؽ�ó �ε�
	CTexture* pTex = CResourceManager::GetInst()->LoadTexture(L"DefaultDesc", L"texture\\UI\\Description\\DefaultDesc.png");
	m_vecPetDesc.push_back(pTex);

	pTex = CResourceManager::GetInst()->LoadTexture(L"ChocoDropDesc", L"texture\\UI\\Description\\ChocoDropDesc.png");
	m_vecPetDesc.push_back(pTex);

	pTex = CResourceManager::GetInst()->LoadTexture(L"MilkBottleDesc", L"texture\\UI\\Description\\MilkBottleDesc.png");
	m_vecPetDesc.push_back(pTex);

	pTex = CResourceManager::GetInst()->LoadTexture(L"FlowerPodDesc", L"texture\\UI\\Description\\FlowerPodDesc.png");
	m_vecPetDesc.push_back(pTex);

	pTex = CResourceManager::GetInst()->LoadTexture(L"SnowGlobeDesc", L"texture\\UI\\Description\\SnowGlobeDesc.png");
	m_vecPetDesc.push_back(pTex);
}

void CSelectLevel::ChangeLoadLevel()
{
	CLoadLevel* pLevel = dynamic_cast<CLoadLevel*>(CLevelManager::GetInst()->GetLevel((UINT)LEVEL_TYPE::LOAD));

	pLevel->SetCookieType(m_eCookieType);
	pLevel->SetPetType(m_ePetType);
	pLevel->SetMatchBonus(m_bMatchBonus);

	ChangeLevel(LEVEL_TYPE::LOAD);
}
