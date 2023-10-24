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

	//배경, 버튼 텍스쳐 불러오기
	CTexture* pSelectPanelTex = CResourceManager::GetInst()->LoadTexture(L"SelectPanel", L"texture\\UI\\Select_Panel.png");
	CTexture* pPlayBtnTex = CResourceManager::GetInst()->LoadTexture(L"SelectPlayBtn", L"texture\\UI\\Select_PlayBtn.png");

	//선택창 배경 설정
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	m_pPanelUI = new CPanelUI;
	m_pPanelUI->SetIdleTex(pSelectPanelTex);
	m_pPanelUI->SetPos(Vec2(0.f, 0.f));

	//애니메이션 재생용 투명 패널 생성해 애니메이터 생성, 재생할 애니메이션 불러오기
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

	//버튼 설정
	//플레이 버튼
	m_pPlayBtn = new CButtonUI;
	m_pPlayBtn->SetIdleTex(pPlayBtnTex);
	m_pPlayBtn->SetPos(vResolution / 2.f + Vec2(265.f, 213.f));
	m_pPlayBtn->SetDelegate(this,(DELEGATE) &CSelectLevel::ChangeLoadLevel);
	m_pPanelUI->AddChildUI(m_pPlayBtn);

	//뒤로가기 버튼

	m_pBackBtn = new CButtonUI;
	CTexture* pBackBtnTex = CResourceManager::GetInst()->LoadTexture(L"BackBtn", L"texture\\UI\\BackBtn.png");
	m_pBackBtn->SetIdleTex(pBackBtnTex);
	m_pBackBtn->SetPos(Vec2(5.f, 5.f));
	m_pBackBtn->SetCallBack(&ReturnToIntroLevel);
	m_pPanelUI->AddChildUI(m_pBackBtn);

	//쿠키, 펫 정보 벡터 채우기
	LoadCookieInfo();
	LoadPetInfo();

	//쿠키, 펫 버튼 만들기
	CreateCookieBtn();
	CreatePetBtn();

	//쿠키, 펫 디스크립션 만들기
	LoadCookieDesc();
	LoadPetDesc();

	//MatchBonus 만들기
	m_pMatchBonus = new CPanelUI;
	m_pMatchBonus->SetIdleTex(nullptr);
	m_pMatchBonus->SetPos(Vec2(940.f, 130.f));
	m_pPanelUI->AddChildUI(m_pMatchBonus);

	//Panel UI 레이어에 등록
	AddObject(m_pPanelUI, LAYER::UI);

	//디폴트로 용감한쿠키, 초코방울 선택되어있음
	m_eCookieType = SELECT_COOKIE::GINGERBRAVE;
	m_ePetType = SELECT_PET::CHOCODROP;

	m_pCookieAnim->GetAnimator()->Play(L"GingerBrave", true);
	m_pPetAnim->GetAnimator()->Play(L"ChocoDrop", true);

	//BGM 재생
	m_pBGM = CResourceManager::GetInst()->LoadSound(L"BGM_Select", L"sound\\BGM\\BGM_Select.wav");
	m_pBGM->PlayToBGM(true);

}

void CSelectLevel::tick()
{
	//선택한 쿠키 정보창 띄우기
	m_pCookieDesc->SetIdleTex(m_vecCookieDesc[(int)m_eCookieType]);
	
	//선택한 펫 정보창 띄우기
	m_pPetDesc->SetIdleTex(m_vecPetDesc[(int)m_ePetType]);

	//선택한 쿠키와 펫이 짝꿍이면 Matchbonus 활성화
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
	//용감한 쿠키
	tCookieInfo tCookie = {};
	tCookie.strKey = L"GingerBrave_Preview";
	tCookie.strPreviewPath = L"texture\\Preview\\pGingerBrave.png";
	tCookie.eCookieType = SELECT_COOKIE::GINGERBRAVE;
	m_vecCookieInfo.push_back(tCookie);

	//우유맛 쿠키
	tCookie = {};
	tCookie.strKey = L"Milk_Preview";
	tCookie.strPreviewPath = L"texture\\Preview\\pMilk.png";
	tCookie.eCookieType = SELECT_COOKIE::MILK;
	m_vecCookieInfo.push_back(tCookie);

	//요정맛 쿠키
	tCookie = {};
	tCookie.strKey = L"Fairy_Preview";
	tCookie.strPreviewPath = L"texture\\Preview\\pFairy.png";
	tCookie.eCookieType = SELECT_COOKIE::FAIRY;
	m_vecCookieInfo.push_back(tCookie);

	//눈설탕맛 쿠키
	tCookie = {};
	tCookie.strKey = L"SnowSugar_Preview";
	tCookie.strPreviewPath = L"texture\\Preview\\pSnowSugar.png";
	tCookie.eCookieType = SELECT_COOKIE::SNOWSUGAR;
	m_vecCookieInfo.push_back(tCookie);
}

void CSelectLevel::LoadPetInfo()
{
	//초코방울(용감한 쿠키)
	tPetInfo tPet = {};
	tPet.strKey = L"ChocoDrop_Preview";
	tPet.strPreviewPath = L"texture\\Preview\\pPetChocoDrop.png";
	tPet.ePetType = SELECT_PET::CHOCODROP;
	m_vecPetInfo.push_back(tPet);

	//우유보틀엔젤(우유맛 쿠키)
	tPet = {};
	tPet.strKey = L"MilkBottle_Preview";
	tPet.strPreviewPath = L"texture\\Preview\\pPetMilkBottle.png";
	tPet.ePetType = SELECT_PET::MILKBOTTLE;
	m_vecPetInfo.push_back(tPet);

	//꽃봉오리(요정맛 쿠키)
	tPet = {};
	tPet.strKey = L"FlowerPod_Preview";
	tPet.strPreviewPath = L"texture\\Preview\\pPetFlowerPod.png";
	tPet.ePetType = SELECT_PET::FLOWERPOD;
	m_vecPetInfo.push_back(tPet);

	//스노우 글로브(눈설탕맛 쿠키)
	tPet = {};
	tPet.strKey = L"SnowGlobe_Preview";
	tPet.strPreviewPath = L"texture\\Preview\\pPetSnowGlobe.png";
	tPet.ePetType = SELECT_PET::SNOWGLOBE;
	m_vecPetInfo.push_back(tPet);
}

void CSelectLevel::CreateCookieBtn()
{
	//쿠키선택 버튼 만들기
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
	//펫선택 버튼 만들기
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
	//쿠키 정보창 만들기
	m_pCookieDesc = new CPanelUI;
	m_pCookieDesc->SetPos(Vec2(478.f, 100.f));

	m_pPanelUI->AddChildUI(m_pCookieDesc);

	//디스크립션 벡터에 텍스처 로드
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
	//펫 정보창 만들기
	m_pPetDesc = new CPanelUI;
	m_pPetDesc->SetPos(Vec2(478.f, 425.f));

	m_pPanelUI->AddChildUI(m_pPetDesc);

	//디스크립션 벡터에 텍스처 로드
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
