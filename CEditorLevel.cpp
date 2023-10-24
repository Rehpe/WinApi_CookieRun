#include "pch.h"
#include "CEditorLevel.h"
#include "CEngine.h"
#include "CCamera.h"
#include "CPlayer.h"
#include "CGround.h"
#include "CPlatform.h"
#include "CCollisionManager.h"
#include "CKeyManager.h"
#include "CObstacle.h"
#include "CBackground.h"
#include "CResourceManager.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CButtonUI.h"
#include "CTexture.h"
#include "CPreview.h"
#include "CPathManager.h"
#include "CMapTileBtn.h"
#include "CGrid.h"
#include "CJelly.h"
#include "CLevelManager.h"
#include "CGingerBraveCookie.h"
#include "CMilkCookie.h"
#include "CPet.h"
#include "CPetChocoDrop.h"
#include "CFSM.h"

void ReturntoIntroLevel()
{
	ChangeLevel(LEVEL_TYPE::INTRO);
}


CEditorLevel::CEditorLevel()
	: m_eMode(EDITOR_MODE::TILE)
	, m_pPreview(nullptr)
	, m_pPanelUI(nullptr)
	, m_pBlankPanelUI(nullptr)
	, m_WorkList{}
{
	SetName(L"EditorLevel");
}

CEditorLevel::~CEditorLevel()
{
}

void CEditorLevel::init()
{
	m_pBlankPanelUI = nullptr;

	//플레이어, 펫 생성
	//CreatePlayer();

	//카메라가 보고 있는 위치를 해상도의 중심으로 설정(좌상단의 좌표가 0,0이 될 수 있게)
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	//그리드 칸 생성
	CreateGrid(700, 30);

	//UI 중 Grid만 골라 Grid 벡터에 넣어줌
	for (size_t i = 0; i < GetLayer(LAYER::UI).size(); i++)
	{
		CGrid* pGrid = dynamic_cast<CGrid*>(GetLayer(LAYER::UI)[i]);

		if (pGrid != nullptr)
			m_vecGrid.push_back(pGrid);
	}

	//배경 이미지 재생
	CObj* pBackground = new CBackground;
	pBackground->SetPos(Vec2(0.f, 0.f));
	static_cast<CBackground*>(pBackground)->init();
	AddObject(pBackground, LAYER::BACKGROUND);

	CObj* pGround = new CGround;
	pGround->SetPos(Vec2(200.f, 670.f));
	AddObject(pGround, LAYER::GROUND);


	//UI생성

	//UI 텍스쳐 불러오기
	CTexture* pSaveBtnTex = CResourceManager::GetInst()->LoadTexture(L"SaveBtn", L"texture\\UI\\saveBtn.png");
	CTexture* pLoadBtnTex = CResourceManager::GetInst()->LoadTexture(L"LoadBtn", L"texture\\UI\\loadBtn.png");
	CTexture* pPanelTex = CResourceManager::GetInst()->LoadTexture(L"Textbox", L"texture\\UI\\textbox1.png");
	CTexture* pBgPanelTex = CResourceManager::GetInst()->LoadTexture(L"BgPanel", L"texture\\UI\\backgroundPanel.png");
	CTexture* pGroundBtnTex = CResourceManager::GetInst()->LoadTexture(L"GroundBtn", L"texture\\UI\\Edit_GroundBtn.png");
	CTexture* pFixedObsBtnTex = CResourceManager::GetInst()->LoadTexture(L"FixedObsBtn", L"texture\\UI\\Edit_FixedObsBtn.png");
	CTexture* pAnimObsBtnTex = CResourceManager::GetInst()->LoadTexture(L"AnimObsBtn", L"texture\\UI\\Edit_AnimObsBtn.png");
	CTexture* pJellyBtnTex = CResourceManager::GetInst()->LoadTexture(L"JellyBtn", L"texture\\UI\\Edit_JellyBtn.png");
	CTexture* pOtherBtnTex = CResourceManager::GetInst()->LoadTexture(L"OtherBtn", L"texture\\UI\\Edit_OtherBtn.png");
	CTexture* pPage1BtnTex = CResourceManager::GetInst()->LoadTexture(L"Page1Btn", L"texture\\UI\\Page_1Btn.png");
	CTexture* pPage2BtnTex = CResourceManager::GetInst()->LoadTexture(L"Page2Btn", L"texture\\UI\\Page_2Btn.png");
	CTexture* pPage3BtnTex = CResourceManager::GetInst()->LoadTexture(L"Page3Btn", L"texture\\UI\\Page_3Btn.png");
	CTexture* pPage4BtnTex = CResourceManager::GetInst()->LoadTexture(L"Page4Btn", L"texture\\UI\\Page_4Btn.png");
	CTexture* pPage5BtnTex = CResourceManager::GetInst()->LoadTexture(L"Page5Btn", L"texture\\UI\\Page_5Btn.png");
	CTexture* pGridTex = CResourceManager::GetInst()->LoadTexture(L"GridTex", L"texture\\UI\\newGrid.png");

	//CUI* newGrid = new CGrid;
	//newGrid->SetIdleTex(pGridTex);
	//newGrid->SetPos(Vec2(0.f, 0.f));
	//AddObject(newGrid, LAYER::UI);

	// Panel UI 설정
	m_pPanelUI = new CPanelUI;
	m_pPanelUI->SetIdleTex(pPanelTex);
	m_pPanelUI->SetPos(Vec2(vResolution.x - (pPanelTex->GetWidth()),0.f));

	// Tile Save 버튼
	CButtonUI* pSaveBtn = new CButtonUI;
	pSaveBtn->SetIdleTex(pSaveBtnTex);
	Vec2 vUIPos = Vec2(70.f, pPanelTex->GetHeight() - 70.f);
	pSaveBtn->SetPos(vUIPos);

	//Load 버튼
	CButtonUI* pLoadBtn = pSaveBtn->Clone();
	pLoadBtn->SetIdleTex(pLoadBtnTex);
	pLoadBtn->SetPos(Vec2(pPanelTex->GetWidth() - (70.f + pLoadBtnTex->GetWidth()), pPanelTex->GetHeight() - 70.f));

	// Save, Load 버튼을 Panel 의 자식으로 추가
	m_pPanelUI->AddChildUI(pSaveBtn);
	m_pPanelUI->AddChildUI(pLoadBtn);

	//Delegate부여
	pSaveBtn->SetDelegate(this, (DELEGATE)&CEditorLevel::SaveMap);
	pLoadBtn->SetDelegate(this, (DELEGATE)&CEditorLevel::LoadMap);

	//====맵타일 카테고리 버튼 생성====
	float fBtnWidth = (float)(pGroundBtnTex->GetWidth());

	CButtonUI* pGroundBtn = new CButtonUI;
	pGroundBtn->SetIdleTex(pGroundBtnTex);
	Vec2 vBtnPos = Vec2(23.f, 30.f);
	pGroundBtn->SetPos(vBtnPos);

	CButtonUI* pFixedObsBtn = pGroundBtn->Clone();
	pFixedObsBtn->SetIdleTex(pFixedObsBtnTex);
	pFixedObsBtn->SetPos(vBtnPos + Vec2(fBtnWidth,0.f));

	CButtonUI* pAnimObsBtn = pGroundBtn->Clone();
	pAnimObsBtn->SetIdleTex(pAnimObsBtnTex);
	pAnimObsBtn->SetPos(vBtnPos + Vec2(fBtnWidth*2, 0.f));

	CButtonUI* pJellyBtn = pGroundBtn->Clone();
	pJellyBtn->SetIdleTex(pJellyBtnTex);
	pJellyBtn->SetPos(vBtnPos + Vec2(fBtnWidth*3, 0.f));

	CButtonUI* pOtherBtn = pGroundBtn->Clone();
	pOtherBtn->SetIdleTex(pOtherBtnTex);
	pOtherBtn->SetPos(vBtnPos + Vec2(fBtnWidth*4, 0.f));

	//Panel의 자식으로 추가
	m_pPanelUI->AddChildUI(pGroundBtn);
	m_pPanelUI->AddChildUI(pFixedObsBtn);
	m_pPanelUI->AddChildUI(pAnimObsBtn);
	m_pPanelUI->AddChildUI(pJellyBtn);
	m_pPanelUI->AddChildUI(pOtherBtn);

	//Delegate 부여
	pGroundBtn->SetStartCount(0);
	pGroundBtn->SetRelativePath(L"ground.info");
	pGroundBtn->SetDoubleParamDelegate(CLevelManager::GetInst()->GetCurLevel(), (DOUBLEPARAMDELEGATE)&CEditorLevel::CreateMapTileBtn);

	pFixedObsBtn->SetStartCount(0);
	pFixedObsBtn->SetRelativePath(L"fixed_obs.info");
	pFixedObsBtn->SetDoubleParamDelegate(CLevelManager::GetInst()->GetCurLevel(), (DOUBLEPARAMDELEGATE)&CEditorLevel::CreateMapTileBtn);

	pAnimObsBtn->SetStartCount(0);
	pAnimObsBtn->SetRelativePath(L"anim_obs.info");
	pAnimObsBtn->SetDoubleParamDelegate(CLevelManager::GetInst()->GetCurLevel(), (DOUBLEPARAMDELEGATE)&CEditorLevel::CreateMapTileBtn);

	pJellyBtn->SetStartCount(0);
	pJellyBtn->SetRelativePath(L"jelly.info");
	pJellyBtn->SetDoubleParamDelegate(CLevelManager::GetInst()->GetCurLevel(), (DOUBLEPARAMDELEGATE)&CEditorLevel::CreateMapTileBtn);

	pOtherBtn->SetStartCount(0);
	pOtherBtn->SetRelativePath(L"ground.info");
	pOtherBtn->SetDoubleParamDelegate(CLevelManager::GetInst()->GetCurLevel(), (DOUBLEPARAMDELEGATE)&CEditorLevel::CreateMapTileBtn);


	//====페이지 버튼 생성====
	Vec2 vInnerPadding = Vec2(10.f, 0.f);
	
	m_pPage1Btn = new CButtonUI;
	m_pPage1Btn->SetIdleTex(pPage1BtnTex);
	Vec2 vPos = Vec2((pPanelTex->GetWidth() - ((pPage1BtnTex->GetWidth() * 5) + (vInnerPadding.x * 4))) / 2.f, pPanelTex->GetHeight() - 120.f);
	m_pPage1Btn->SetPos(vPos);

	m_pPage2Btn = m_pPage1Btn->Clone();
	m_pPage2Btn->SetIdleTex(pPage2BtnTex);
	m_pPage2Btn->SetPos(Vec2(vPos.x + (pPage1BtnTex->GetWidth() + vInnerPadding.x)*1, vPos.y));

	m_pPage3Btn = m_pPage1Btn->Clone();
	m_pPage3Btn->SetIdleTex(pPage3BtnTex);
	m_pPage3Btn->SetPos(Vec2(vPos.x + (pPage1BtnTex->GetWidth() + vInnerPadding.x) * 2, vPos.y));

	m_pPage4Btn = m_pPage1Btn->Clone();
	m_pPage4Btn->SetIdleTex(pPage4BtnTex);
	m_pPage4Btn->SetPos(Vec2(vPos.x + (pPage1BtnTex->GetWidth() + vInnerPadding.x) * 3, vPos.y));

	m_pPage5Btn = m_pPage1Btn->Clone();
	m_pPage5Btn->SetIdleTex(pPage5BtnTex);
	m_pPage5Btn->SetPos(Vec2(vPos.x + (pPage1BtnTex->GetWidth() + vInnerPadding.x) * 4, vPos.y));

	//Panel의 자식으로 추가
	m_pPanelUI->AddChildUI(m_pPage1Btn);
	m_pPanelUI->AddChildUI(m_pPage2Btn);
	m_pPanelUI->AddChildUI(m_pPage3Btn);
	m_pPanelUI->AddChildUI(m_pPage4Btn);
	m_pPanelUI->AddChildUI(m_pPage5Btn);

	m_pBackBtn = new CButtonUI;
	CTexture* pBackBtnTex = CResourceManager::GetInst()->LoadTexture(L"BackBtn", L"texture\\UI\\BackBtn.png");
	m_pBackBtn->SetIdleTex(pBackBtnTex);
	m_pBackBtn->SetPos(Vec2(5.f, 5.f));
	m_pBackBtn->SetCallBack(&ReturntoIntroLevel);
	AddObject(m_pBackBtn, LAYER::UI);

	//====CreateButton====
	
	//초기 맵타일버튼 15개 생성
	CreateMapTileBtn(L"ground.info", 0);

	//구성된 PANEL UI 레이어에 추가
	AddObject(m_pPanelUI, LAYER::UI);

	m_pPanelUI->m_fRatio = 0.8f;//0.8f;
	m_pBlankPanelUI->m_fRatio = 0.5f; //0.5f;

	//충돌관계 설정
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::GROUND);
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::OBSTACLE);
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::JELLY);


}

void CEditorLevel::tick()
{
	if (GetPlayer() == nullptr)
	{
		//카메라가 있는 경우 캐릭터 위치에 따라 카메라 Look 변경
		Vec2 vResolution = CEngine::GetInst()->GetResolution();
		Vec2 vLook = CCamera::GetInst()->GetLook();
		CCamera::GetInst()->SetLook(Vec2(vResolution / 2.f));
	}

	if (GetPlayer())
	{
		GetPlayer()->SetInvincible(true);
		Vec2 vOffset = Vec2(450.f, -280.f);
		CCamera::GetInst()->SetLook(Vec2((GetPlayer()->GetPos()).x + vOffset.x, 300.f));
	}


	//실행취소
	if (IsTap(KEY::Z))
	{
		if (!m_WorkList.empty())
		{
			m_WorkList.front()->SetDead();
			m_WorkList.pop_front();
		}
	}
	if (IsTap(KEY::_3))
	{
		if(!GetPlayer() && !GetPet())
		CreatePlayer();

		if(GetPlayer() && GetPet())
		{
		DeleteObject(LAYER::PLAYER);
		DeleteObject(LAYER::PET);

		CreatePlayer();
		}
	}
	
	if (IsTap(KEY::LCTRL))
	{
		SetColliderRender(true);
	}

	CLevel::tick();
	
}

void CEditorLevel::Enter()
{
	//초기 오브젝트 설정
	init();
}

void CEditorLevel::Exit()
{
	DeleteAllObject();
}

void CEditorLevel::object_update()
{
	if (IsRelease(KEY::LBTN))
	{
		//맵타일 버튼을 클릭하여 생성할 클래스, 레이어 정보를 받아왔는지 확인(없다면 return)
		if (m_eCreateClassType == CLASS_TYPE::NONE)
			return;

		// 마우스 위치를 받아와서 실제좌표로 변환	
		Vec2 vMousePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		//Vec2 vGridPos = Vec2((float)vMousePos.x - ((int)vMousePos.x % 30), (float)vMousePos.y - ((int)vMousePos.y % 30));

		int iCol = (int)vMousePos.x / GRID_SIZE;
		int iRow = (int)vMousePos.y / GRID_SIZE;

		if (0.f <= vMousePos.x && iCol < GetGridXCount()
			&& 0.f <= vMousePos.y && iRow < GetGridYCount())
		{
			int iIdx = iRow * GetGridXCount() + iCol;

			Vec2 vGridPos = ((CGrid*)m_vecGrid[iIdx])->GetPos();
			
			CMapTile* pObj = nullptr;
			LAYER eLayer = LAYER::DEFAULT;

			switch (m_eCreateClassType)
			{
			case CLASS_TYPE::GROUND:
			{
				pObj = new CGround;
				pObj->SetTexture(m_pPreview->GetPreviewTexture());
				pObj->init();
				//땅일 경우 보정
				vGridPos.y = 670.f;
				eLayer = LAYER::GROUND;
			}
				break;
			case CLASS_TYPE::PLATFORM:
			{
				pObj = new CPlatform;
				pObj->SetTexture(m_pPreview->GetPreviewTexture());
				pObj->init();
				eLayer = LAYER::GROUND;
			}
			break;
			case CLASS_TYPE::FIXED_OBSTACLE:
			{
				pObj = new CObstacle;
				pObj->SetTexture(m_pPreview->GetPreviewTexture());
				static_cast<CObstacle*>(pObj)->init(OBS_TYPE::FIXED, false);
				eLayer = LAYER::OBSTACLE;
			}
				break;
			case CLASS_TYPE::JELLY:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::JELLY);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::ITEM_BEARJELLYPARTY:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::ITEM_BEARJELLYPARTY);
				eLayer = LAYER::JELLY;
			}
				break;
			case CLASS_TYPE::ITEM_BIGGER:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::ITEM_BIGGER);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::ITEM_BONUS:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::ITEM_BONUS);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::ITEM_BOOSTER:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::ITEM_BOOSTER);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::ITEM_COINMAGIC:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::ITEM_COINMAGIC);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::ITEM_MAGNET:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::ITEM_MAGNET);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::ITEM_ICEBEARJELLYPARTY:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::ITEM_ICEBEARJELLYPARTY);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::ITEM_MINIPOTION:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::ITEM_MINIPOTION);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::ITEM_BIGPOTION:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::ITEM_BIGPOTION);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::JELLY_YELLOWBEAR:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::JELLY_YELLOWBEAR);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::JELLY_PINKBEAR:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::JELLY_PINKBEAR);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::JELLY_ICEBEAR:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::JELLY_ICEBEAR);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::JELLY_BIGBEAR:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::JELLY_BIGBEAR);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::JELLY_RAINBOWBEAR:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::JELLY_RAINBOWBEAR);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::BONUS_RAINBOW:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::BONUS_RAINBOW);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::BONUS_GOLD:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::BONUS_GOLD);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::COIN_SILVER:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::COIN_SILVER);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::COIN_GOLD:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::COIN_GOLD);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::COIN_BIGSILVER:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::COIN_BIGSILVER);
				eLayer = LAYER::JELLY;
			}
			break;
			case CLASS_TYPE::COIN_BIGGOLD:
			{
				pObj = new CJelly;
				static_cast<CJelly*>(pObj)->init(ITEM_TYPE::COIN_BIGGOLD);
				eLayer = LAYER::JELLY;
			}
			break;

			case CLASS_TYPE::ANIM_OBS1:
			{
				pObj = new CObstacle;
				static_cast<CObstacle*>(pObj)->init(OBS_TYPE::ANIM_1,false);
				eLayer = LAYER::OBSTACLE;
			}
			break;
			case CLASS_TYPE::ANIM_OBS2:
			{
				pObj = new CObstacle;
				static_cast<CObstacle*>(pObj)->init(OBS_TYPE::ANIM_2, false);
				eLayer = LAYER::OBSTACLE;
			}
			break;
			case CLASS_TYPE::ANIM_OBS3:
			{
				pObj = new CObstacle;
				static_cast<CObstacle*>(pObj)->init(OBS_TYPE::ANIM_3, false);
				eLayer = LAYER::OBSTACLE;
			}
			break;
			case CLASS_TYPE::ANIM_OBS4:
			{
				pObj = new CObstacle;
				static_cast<CObstacle*>(pObj)->init(OBS_TYPE::ANIM_4, false);
				eLayer = LAYER::OBSTACLE;
			}
			break;
			case CLASS_TYPE::ANIM_OBS5:
			{
				pObj = new CObstacle;
				static_cast<CObstacle*>(pObj)->init(OBS_TYPE::ANIM_5, false);
				eLayer = LAYER::OBSTACLE;
			}
			break;
			case CLASS_TYPE::ANIM_OBS6:
			{
				pObj = new CObstacle;
				static_cast<CObstacle*>(pObj)->init(OBS_TYPE::ANIM_6, false);
				eLayer = LAYER::OBSTACLE;
			}
			break;
			case CLASS_TYPE::ANIM_OBS7:
			{
				pObj = new CObstacle;
				static_cast<CObstacle*>(pObj)->init(OBS_TYPE::ANIM_7, false);
				eLayer = LAYER::OBSTACLE;
			}
			break;
			case CLASS_TYPE::ANIM_OBS8:
			{
				pObj = new CObstacle;
				static_cast<CObstacle*>(pObj)->init(OBS_TYPE::ANIM_8, false);
				eLayer = LAYER::OBSTACLE;
			}
			break;

			default:
				break;
			}
			//오브젝트 생성
			Vec2 vObjScale = Vec2(pObj->GetScale());
			Instantiate(pObj, vGridPos, eLayer);

			//작업목록에도 추가(stack)
			m_WorkList.push_front(pObj);
		}
	}
}


void CEditorLevel::LoadBtnInfo(const wstring& _strRelativePath)
{
	//기존 벡터를 비우고 시작한다
	m_vecBtnInfo.clear();

	//PathManager에서 기본 경로를 가져온다
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();

	//인자로 들어온 상대경로를 붙여 애니메이션을 불러올 최종 경로를 완성한다.
	strFilePath += _strRelativePath;

	//파일입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	//파일 포인터가 nullptr라면 파일 생성이 제대로 되지 않은 것으로 리턴
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"BtnInfo Load 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"버튼 정보 불러오기 실패", MB_OK);
		return;
	}

	//불러올 변수들을 선언해준다
	size_t iCount = 0;

	//데이터들을 한번에 읽어올 2바이트 문자열 256칸 버퍼를 만든다.
	while (true)
	{
		wchar_t szBuffer[256] = {};
		fwscanf_s(pFile, L"%s", szBuffer, 256);

		//====읽어들인 문자열(Buffer)이 필드명과 같은지 체크하고, 같다면 구조체 정보로 셋팅한다===

		if (!wcscmp(szBuffer, L"[MAPTILE_COUNT]"))
		{
			fwscanf_s(pFile, L"%zd", &iCount);

			//iCount만큼 반복문을 돌려 이하 값들을 읽어온다
			for (size_t i = 0; i < iCount; i++)
			{
				tBtnInfo BtnInfo = {};

				while (true)
				{
					//데이터들을 한번에 읽어올 2바이트 문자열 256칸 버퍼를 만든다.
					wchar_t szBuffer[256] = {};
					fwscanf_s(pFile, L"%s", szBuffer, 256);

					//이미지 이름
					if (!wcscmp(szBuffer, L"[MAPTILE_IMAGE_KEY]"))
					{
						fwscanf_s(pFile, L"%s", szBuffer, 256);
						BtnInfo.strKey = szBuffer;
					}
					//이미지 경로
					else if (!wcscmp(szBuffer, L"[MAPTILE_IMAGE_PATH]"))
					{
						fwscanf_s(pFile, L"%s", szBuffer, 256);
						BtnInfo.strRelativePath = szBuffer;
					}
					//생성할 클래스
					else if (!wcscmp(szBuffer, L"[MAPTILE_CLASS]"))
					{
						fwscanf_s(pFile, L"%i", &BtnInfo.iClassType);
						break;
					}
				}
				//반복문을 돌며 차례대로 가져온 frm 구조체를 애니메이션 프레임 벡터에 푸시백해준다.
				m_vecBtnInfo.push_back(BtnInfo);
			}
			break;
		}
	}

	//파일 로딩을 마치면 스트림을 닫아준다. 
	fclose(pFile);
}

void CEditorLevel::CreateMapTileBtn(const wstring& _strRelativePath, int _iStartCount)
{
	//기존에 생성된 BlankPanel이 있다면, 자식UI를 모두 지우고, 부모의 자식 목록에서 본인도 완전히 삭제한다.
	if (m_pBlankPanelUI != nullptr)
	{
		m_pBlankPanelUI->ClearChildUI();
		m_pPanelUI->GetChildUI().pop_back();
		delete m_pBlankPanelUI;
	}

	//BlankPanelUI 생성
	CTexture* pBgPanelTex = CResourceManager::GetInst()->LoadTexture(L"BgPanel", L"texture\\UI\\backgroundPanel.png");

	m_pBlankPanelUI = new CPanelUI;
	m_pBlankPanelUI->SetIdleTex(pBgPanelTex);
	float fPos = (m_pPanelUI->GetIdleTex()->GetWidth() - m_pBlankPanelUI->GetIdleTex()->GetWidth()) / 2.f;
	m_pBlankPanelUI->SetPos(Vec2(fPos, 70.f));

	//맵타일 정보 불러오기
	LoadBtnInfo(_strRelativePath);

	//맵타일버튼 15개 생성 후 불러온 정보를 바탕으로 버튼정보 채우기
	Vec2 vInnerPadding = Vec2(50.f, 10.f);

	Vec2 vLeftTop = Vec2((m_pBlankPanelUI->GetIdleTex()->GetWidth() - (BTN_SIZE * 3 + vInnerPadding.x * 2)) / 2.f, (m_pBlankPanelUI->GetIdleTex()->GetHeight() - (BTN_SIZE * 5 + vInnerPadding.y * 4)) / 2.f);
	
	for (int iRow = 0; iRow < 5; iRow++)
	{
		for (int iCol = 0; iCol < 3; iCol++)
		{
			int iIndexNum = _iStartCount + iCol + (3 * iRow);

			CMapTileBtn* pButton = new CMapTileBtn;
			pButton->SetPos(Vec2(vLeftTop + Vec2((BTN_SIZE * iCol) + vInnerPadding.x * iCol, float(BTN_SIZE * iRow) + vInnerPadding.y * iRow)));

			if (m_vecBtnInfo.size() >= (iIndexNum) + 1)
			{

				CTexture* pTexture = CResourceManager::GetInst()->LoadTexture(m_vecBtnInfo[iIndexNum].strKey, m_vecBtnInfo[iIndexNum].strRelativePath);
				pButton->SetIdleTex(pTexture);
				//pButton->SetKey(m_vecBtnInfo[iIndexNum].strKey);
				//pButton->SetRelativePath(m_vecBtnInfo[iIndexNum].strRelativePath);
				pButton->SetClassType(m_vecBtnInfo[iIndexNum].iClassType);
				pButton->SetDelegate(pButton, (DELEGATE)&CMapTileBtn::selectObject);
			}

			m_pBlankPanelUI->AddChildUI(pButton);
		}
	}
	
	m_pPanelUI->AddChildUI(m_pBlankPanelUI);

	//이후 페이지 번호에 불러온 정보를 바탕으로 페이지 분배
	m_pPage1Btn->SetStartCount(0);
	m_pPage1Btn->SetRelativePath(_strRelativePath);
	m_pPage1Btn->SetDoubleParamDelegate(CLevelManager::GetInst()->GetCurLevel(), (DOUBLEPARAMDELEGATE)&CEditorLevel::CreateMapTileBtn);

	m_pPage2Btn->SetStartCount(15);
	m_pPage2Btn->SetRelativePath(_strRelativePath);
	m_pPage2Btn->SetDoubleParamDelegate(CLevelManager::GetInst()->GetCurLevel(), (DOUBLEPARAMDELEGATE)&CEditorLevel::CreateMapTileBtn);

	m_pPage3Btn->SetStartCount(30);
	m_pPage3Btn->SetRelativePath(_strRelativePath);
	m_pPage3Btn->SetDoubleParamDelegate(CLevelManager::GetInst()->GetCurLevel(), (DOUBLEPARAMDELEGATE)&CEditorLevel::CreateMapTileBtn);

	m_pPage4Btn->SetStartCount(45);
	m_pPage4Btn->SetRelativePath(_strRelativePath);
	m_pPage4Btn->SetDoubleParamDelegate(CLevelManager::GetInst()->GetCurLevel(), (DOUBLEPARAMDELEGATE)&CEditorLevel::CreateMapTileBtn);

	m_pPage5Btn->SetStartCount(60);
	m_pPage5Btn->SetRelativePath(_strRelativePath);
	m_pPage5Btn->SetDoubleParamDelegate(CLevelManager::GetInst()->GetCurLevel(), (DOUBLEPARAMDELEGATE)&CEditorLevel::CreateMapTileBtn);
}

void CEditorLevel::SaveMap()
{
	// open a file name
	OPENFILENAME ofn = {};

	//PathManager에서 기본 경로를 가져온다
	wstring strFileFolderPath = CPathManager::GetInst()->GetContentPath();
	strFileFolderPath += L"map\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Map\0*.map\0ALL\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strFileFolderPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetSaveFileName(&ofn))
		return;

	//파일입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"wb");

	//파일 포인터가 nullptr라면 파일 생성이 제대로 되지 않은 것으로 리턴
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Map Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"저장 실패", MB_OK);
		return;
	}

	//==========Ground Layer 저장============
	
	//생성된 Ground Obj 갯수 저장
	UINT iObjCount = GetLayer(LAYER::GROUND).size();

	fwrite(&iObjCount, sizeof(UINT), 1, pFile);

	//iObjCount만큼 반복문을 돌며 프레임 정보를 기록한다
	for (size_t i = 0; i < iObjCount; i++)
	{
		CMapTile* pObj = dynamic_cast<CMapTile*>(GetLayer(LAYER::GROUND)[i]);
		//CTexture* pTex = dynamic_cast<CMapTile*>(pObj)->GetTexture();

		// 위치
		Vec2 vPos = pObj->GetPos();
		fwrite(&vPos, sizeof(Vec2), 1, pFile);

		//오브젝트 텍스쳐의 키값
		wstring strKey = pObj->GetTexture()->GetKey();
		SaveWString(strKey, pFile);

		//오브젝트 텍스쳐의 경로값
		wstring strRelativePath = pObj->GetTexture()->GetRelativePath();
		SaveWString(strRelativePath, pFile);
	}

	//==========Obstacle Layer 저장============

	//생성된 Obstacle Obj 갯수 저장
	iObjCount = GetLayer(LAYER::OBSTACLE).size();

	fwrite(&iObjCount, sizeof(UINT), 1, pFile);

	//iObjCount만큼 반복문을 돌며 프레임 정보를 기록한다
	for (size_t i = 0; i < iObjCount; i++)
	{
		CObstacle* pObj = dynamic_cast<CObstacle*>(GetLayer(LAYER::OBSTACLE)[i]);
		//CTexture* pTex = dynamic_cast<CMapTile*>(pObj)->GetTexture();

		// 위치
		Vec2 vPos = pObj->GetPos();
		fwrite(&vPos, sizeof(Vec2), 1, pFile);

		//오브젝트 텍스쳐의 키값
		wstring strKey = pObj->GetTexture()->GetKey();
		SaveWString(strKey, pFile);

		//오브젝트 텍스쳐의 경로값
		wstring strRelativePath = pObj->GetTexture()->GetRelativePath();
		SaveWString(strRelativePath, pFile);

		//오브젝트의 Obstacle type
		int iObsType = (int)pObj->GetObsType();
		fwrite(&iObsType, sizeof(int), 1, pFile);
	}

	//==========Jelly Layer 저장============
	
	//생성된 Jelly Obj 갯수 저장
	iObjCount = GetLayer(LAYER::JELLY).size();

	fwrite(&iObjCount, sizeof(UINT), 1, pFile);

	//iObjCount만큼 반복문을 돌며 프레임 정보를 기록한다
	for (size_t i = 0; i < iObjCount; i++)
	{
		CJelly* pObj = dynamic_cast<CJelly*>(GetLayer(LAYER::JELLY)[i]);
		//CTexture* pTex = dynamic_cast<CMapTile*>(pObj)->GetTexture();
		
		// 위치
		Vec2 vPos = pObj->GetPos();
		fwrite(&vPos, sizeof(Vec2), 1, pFile);

		//오브젝트 텍스쳐의 키값
		wstring strKey = pObj->GetTexture()->GetKey();
		SaveWString(strKey, pFile);

		//오브젝트 텍스쳐의 경로값
		wstring strRelativePath = pObj->GetTexture()->GetRelativePath();
		SaveWString(strRelativePath, pFile);

		//오브젝트의 Item type
		int iItemType = (int)pObj->GetItemType();
		fwrite(&iItemType, sizeof(int), 1, pFile);
	}

	//파일 저장을 마치면 스트림을 닫아준다.
	fclose(pFile);
}

void CEditorLevel::LoadMap()
{
	// open a file name
	OPENFILENAME ofn = {};

	//PathManager에서 기본 경로를 가져온다
	wstring strFileFolderPath = CPathManager::GetInst()->GetContentPath();
	strFileFolderPath += L"map\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"Map\0*.map\0ALL\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strFileFolderPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetOpenFileName(&ofn))
		return;

	//기존 생성된 모든 객체 삭제
	DeleteObject(LAYER::GROUND);
	DeleteObject(LAYER::OBSTACLE);
	DeleteObject(LAYER::JELLY);

	//파일입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"rb");

	//생성된 Ground Obj 객체 수 불러오기
	UINT iGroundObjCount = 0;
	fread(&iGroundObjCount, sizeof(UINT), 1, pFile);

		//iGroundObjCount만큼 반복문을 돌며 정보 불러오기
		for (size_t i = 0; i < iGroundObjCount; i++)
		{
			CGround* pObj = new CGround;

			// 위치
			Vec2 vPos;
			fread(&vPos, sizeof(Vec2), 1, pFile);

			// 텍스처 이미지 Key와 상대경로
			wstring strKey, strRelativePath;
			LoadWString(strKey, pFile);
			LoadWString(strRelativePath, pFile);
			
			//불러온 정보 대입 후 객체 생성, 초기화
			Instantiate(pObj, vPos, LAYER::GROUND);
			pObj->SetTexture(CResourceManager::GetInst()->LoadTexture(strKey, strRelativePath));
			pObj->init();
		}

		// 생성된 Obstacle Obj 객체 수 불러오기
		UINT iObstacleObjCount = 0;
		fread(&iObstacleObjCount, sizeof(UINT), 1, pFile);

		//iGroundObjCount만큼 반복문을 돌며 정보 불러오기
		for (size_t i = 0; i < iObstacleObjCount; i++)
		{
			CObstacle* pObj = new CObstacle;

			// 위치
			Vec2 vPos;
			fread(&vPos, sizeof(Vec2), 1, pFile);

			// 텍스처 이미지 Key와 상대경로
			wstring strKey, strRelativePath;
			LoadWString(strKey, pFile);
			LoadWString(strRelativePath, pFile);

			//Obstacle Type
			int iObsType = 0;
			fread(&iObsType, sizeof(int), 1, pFile);

			//불러온 정보 대입 후 객체 생성, 초기화
			Instantiate(pObj, vPos, LAYER::OBSTACLE);
			pObj->SetTexture(CResourceManager::GetInst()->LoadTexture(strKey, strRelativePath));
			pObj->init((OBS_TYPE)iObsType, false);
		}

		// 생성된 Jelly Obj 객체 수 불러오기
		UINT iJellyObjCount = 0;
		fread(&iJellyObjCount, sizeof(UINT), 1, pFile);

		//iGroundObjCount만큼 반복문을 돌며 정보 불러오기
		for (size_t i = 0; i < iJellyObjCount; i++)
		{
			CJelly* pObj = new CJelly;

			// 위치
			Vec2 vPos;
			fread(&vPos, sizeof(Vec2), 1, pFile);

			// 텍스처 이미지 Key와 상대경로
			wstring strKey, strRelativePath;
			LoadWString(strKey, pFile);
			LoadWString(strRelativePath, pFile);

			//Item Type
			int iItemType = 0;
			fread(&iItemType, sizeof(int), 1, pFile);

			//불러온 정보 대입 후 객체 생성, 초기화
			Instantiate(pObj, vPos, LAYER::JELLY);
			pObj->SetTexture(CResourceManager::GetInst()->LoadTexture(strKey, strRelativePath));
			pObj->init((ITEM_TYPE)iItemType);
		}
	
		fclose(pFile);
	}

	void CEditorLevel::CreatePlayer()
	{
		//플레이어 생성 및 초기셋팅
		CPlayer* pPlayer = new CGingerBraveCookie;
		pPlayer->SetPos(Vec2(200.f, 300.f));
		//플레이어 오브젝트 배열에 넣어주기
		AddObject(pPlayer, LAYER::PLAYER);
		SetPlayer(pPlayer);
		pPlayer->SetHpReduction(0);
		pPlayer->SetSpeed(0);

		CPet* pPet = new CPetChocoDrop;
		SetPet(pPet);
		AddObject(pPet, LAYER::PET);
	}





	


