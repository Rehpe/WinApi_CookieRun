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

	//�÷��̾�, �� ����
	//CreatePlayer();

	//ī�޶� ���� �ִ� ��ġ�� �ػ��� �߽����� ����(�»���� ��ǥ�� 0,0�� �� �� �ְ�)
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	//�׸��� ĭ ����
	CreateGrid(700, 30);

	//UI �� Grid�� ��� Grid ���Ϳ� �־���
	for (size_t i = 0; i < GetLayer(LAYER::UI).size(); i++)
	{
		CGrid* pGrid = dynamic_cast<CGrid*>(GetLayer(LAYER::UI)[i]);

		if (pGrid != nullptr)
			m_vecGrid.push_back(pGrid);
	}

	//��� �̹��� ���
	CObj* pBackground = new CBackground;
	pBackground->SetPos(Vec2(0.f, 0.f));
	static_cast<CBackground*>(pBackground)->init();
	AddObject(pBackground, LAYER::BACKGROUND);

	CObj* pGround = new CGround;
	pGround->SetPos(Vec2(200.f, 670.f));
	AddObject(pGround, LAYER::GROUND);


	//UI����

	//UI �ؽ��� �ҷ�����
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

	// Panel UI ����
	m_pPanelUI = new CPanelUI;
	m_pPanelUI->SetIdleTex(pPanelTex);
	m_pPanelUI->SetPos(Vec2(vResolution.x - (pPanelTex->GetWidth()),0.f));

	// Tile Save ��ư
	CButtonUI* pSaveBtn = new CButtonUI;
	pSaveBtn->SetIdleTex(pSaveBtnTex);
	Vec2 vUIPos = Vec2(70.f, pPanelTex->GetHeight() - 70.f);
	pSaveBtn->SetPos(vUIPos);

	//Load ��ư
	CButtonUI* pLoadBtn = pSaveBtn->Clone();
	pLoadBtn->SetIdleTex(pLoadBtnTex);
	pLoadBtn->SetPos(Vec2(pPanelTex->GetWidth() - (70.f + pLoadBtnTex->GetWidth()), pPanelTex->GetHeight() - 70.f));

	// Save, Load ��ư�� Panel �� �ڽ����� �߰�
	m_pPanelUI->AddChildUI(pSaveBtn);
	m_pPanelUI->AddChildUI(pLoadBtn);

	//Delegate�ο�
	pSaveBtn->SetDelegate(this, (DELEGATE)&CEditorLevel::SaveMap);
	pLoadBtn->SetDelegate(this, (DELEGATE)&CEditorLevel::LoadMap);

	//====��Ÿ�� ī�װ� ��ư ����====
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

	//Panel�� �ڽ����� �߰�
	m_pPanelUI->AddChildUI(pGroundBtn);
	m_pPanelUI->AddChildUI(pFixedObsBtn);
	m_pPanelUI->AddChildUI(pAnimObsBtn);
	m_pPanelUI->AddChildUI(pJellyBtn);
	m_pPanelUI->AddChildUI(pOtherBtn);

	//Delegate �ο�
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


	//====������ ��ư ����====
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

	//Panel�� �ڽ����� �߰�
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
	
	//�ʱ� ��Ÿ�Ϲ�ư 15�� ����
	CreateMapTileBtn(L"ground.info", 0);

	//������ PANEL UI ���̾ �߰�
	AddObject(m_pPanelUI, LAYER::UI);

	m_pPanelUI->m_fRatio = 0.8f;//0.8f;
	m_pBlankPanelUI->m_fRatio = 0.5f; //0.5f;

	//�浹���� ����
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::GROUND);
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::OBSTACLE);
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::JELLY);


}

void CEditorLevel::tick()
{
	if (GetPlayer() == nullptr)
	{
		//ī�޶� �ִ� ��� ĳ���� ��ġ�� ���� ī�޶� Look ����
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


	//�������
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
	//�ʱ� ������Ʈ ����
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
		//��Ÿ�� ��ư�� Ŭ���Ͽ� ������ Ŭ����, ���̾� ������ �޾ƿԴ��� Ȯ��(���ٸ� return)
		if (m_eCreateClassType == CLASS_TYPE::NONE)
			return;

		// ���콺 ��ġ�� �޾ƿͼ� ������ǥ�� ��ȯ	
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
				//���� ��� ����
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
			//������Ʈ ����
			Vec2 vObjScale = Vec2(pObj->GetScale());
			Instantiate(pObj, vGridPos, eLayer);

			//�۾���Ͽ��� �߰�(stack)
			m_WorkList.push_front(pObj);
		}
	}
}


void CEditorLevel::LoadBtnInfo(const wstring& _strRelativePath)
{
	//���� ���͸� ���� �����Ѵ�
	m_vecBtnInfo.clear();

	//PathManager���� �⺻ ��θ� �����´�
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();

	//���ڷ� ���� ����θ� �ٿ� �ִϸ��̼��� �ҷ��� ���� ��θ� �ϼ��Ѵ�.
	strFilePath += _strRelativePath;

	//���������
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	//���� �����Ͱ� nullptr��� ���� ������ ����� ���� ���� ������ ����
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"BtnInfo Load ����, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"��ư ���� �ҷ����� ����", MB_OK);
		return;
	}

	//�ҷ��� �������� �������ش�
	size_t iCount = 0;

	//�����͵��� �ѹ��� �о�� 2����Ʈ ���ڿ� 256ĭ ���۸� �����.
	while (true)
	{
		wchar_t szBuffer[256] = {};
		fwscanf_s(pFile, L"%s", szBuffer, 256);

		//====�о���� ���ڿ�(Buffer)�� �ʵ��� ������ üũ�ϰ�, ���ٸ� ����ü ������ �����Ѵ�===

		if (!wcscmp(szBuffer, L"[MAPTILE_COUNT]"))
		{
			fwscanf_s(pFile, L"%zd", &iCount);

			//iCount��ŭ �ݺ����� ���� ���� ������ �о�´�
			for (size_t i = 0; i < iCount; i++)
			{
				tBtnInfo BtnInfo = {};

				while (true)
				{
					//�����͵��� �ѹ��� �о�� 2����Ʈ ���ڿ� 256ĭ ���۸� �����.
					wchar_t szBuffer[256] = {};
					fwscanf_s(pFile, L"%s", szBuffer, 256);

					//�̹��� �̸�
					if (!wcscmp(szBuffer, L"[MAPTILE_IMAGE_KEY]"))
					{
						fwscanf_s(pFile, L"%s", szBuffer, 256);
						BtnInfo.strKey = szBuffer;
					}
					//�̹��� ���
					else if (!wcscmp(szBuffer, L"[MAPTILE_IMAGE_PATH]"))
					{
						fwscanf_s(pFile, L"%s", szBuffer, 256);
						BtnInfo.strRelativePath = szBuffer;
					}
					//������ Ŭ����
					else if (!wcscmp(szBuffer, L"[MAPTILE_CLASS]"))
					{
						fwscanf_s(pFile, L"%i", &BtnInfo.iClassType);
						break;
					}
				}
				//�ݺ����� ���� ���ʴ�� ������ frm ����ü�� �ִϸ��̼� ������ ���Ϳ� Ǫ�ù����ش�.
				m_vecBtnInfo.push_back(BtnInfo);
			}
			break;
		}
	}

	//���� �ε��� ��ġ�� ��Ʈ���� �ݾ��ش�. 
	fclose(pFile);
}

void CEditorLevel::CreateMapTileBtn(const wstring& _strRelativePath, int _iStartCount)
{
	//������ ������ BlankPanel�� �ִٸ�, �ڽ�UI�� ��� �����, �θ��� �ڽ� ��Ͽ��� ���ε� ������ �����Ѵ�.
	if (m_pBlankPanelUI != nullptr)
	{
		m_pBlankPanelUI->ClearChildUI();
		m_pPanelUI->GetChildUI().pop_back();
		delete m_pBlankPanelUI;
	}

	//BlankPanelUI ����
	CTexture* pBgPanelTex = CResourceManager::GetInst()->LoadTexture(L"BgPanel", L"texture\\UI\\backgroundPanel.png");

	m_pBlankPanelUI = new CPanelUI;
	m_pBlankPanelUI->SetIdleTex(pBgPanelTex);
	float fPos = (m_pPanelUI->GetIdleTex()->GetWidth() - m_pBlankPanelUI->GetIdleTex()->GetWidth()) / 2.f;
	m_pBlankPanelUI->SetPos(Vec2(fPos, 70.f));

	//��Ÿ�� ���� �ҷ�����
	LoadBtnInfo(_strRelativePath);

	//��Ÿ�Ϲ�ư 15�� ���� �� �ҷ��� ������ �������� ��ư���� ä���
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

	//���� ������ ��ȣ�� �ҷ��� ������ �������� ������ �й�
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

	//PathManager���� �⺻ ��θ� �����´�
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

	//���������
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"wb");

	//���� �����Ͱ� nullptr��� ���� ������ ����� ���� ���� ������ ����
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Map Save ����, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"���� ����", MB_OK);
		return;
	}

	//==========Ground Layer ����============
	
	//������ Ground Obj ���� ����
	UINT iObjCount = GetLayer(LAYER::GROUND).size();

	fwrite(&iObjCount, sizeof(UINT), 1, pFile);

	//iObjCount��ŭ �ݺ����� ���� ������ ������ ����Ѵ�
	for (size_t i = 0; i < iObjCount; i++)
	{
		CMapTile* pObj = dynamic_cast<CMapTile*>(GetLayer(LAYER::GROUND)[i]);
		//CTexture* pTex = dynamic_cast<CMapTile*>(pObj)->GetTexture();

		// ��ġ
		Vec2 vPos = pObj->GetPos();
		fwrite(&vPos, sizeof(Vec2), 1, pFile);

		//������Ʈ �ؽ����� Ű��
		wstring strKey = pObj->GetTexture()->GetKey();
		SaveWString(strKey, pFile);

		//������Ʈ �ؽ����� ��ΰ�
		wstring strRelativePath = pObj->GetTexture()->GetRelativePath();
		SaveWString(strRelativePath, pFile);
	}

	//==========Obstacle Layer ����============

	//������ Obstacle Obj ���� ����
	iObjCount = GetLayer(LAYER::OBSTACLE).size();

	fwrite(&iObjCount, sizeof(UINT), 1, pFile);

	//iObjCount��ŭ �ݺ����� ���� ������ ������ ����Ѵ�
	for (size_t i = 0; i < iObjCount; i++)
	{
		CObstacle* pObj = dynamic_cast<CObstacle*>(GetLayer(LAYER::OBSTACLE)[i]);
		//CTexture* pTex = dynamic_cast<CMapTile*>(pObj)->GetTexture();

		// ��ġ
		Vec2 vPos = pObj->GetPos();
		fwrite(&vPos, sizeof(Vec2), 1, pFile);

		//������Ʈ �ؽ����� Ű��
		wstring strKey = pObj->GetTexture()->GetKey();
		SaveWString(strKey, pFile);

		//������Ʈ �ؽ����� ��ΰ�
		wstring strRelativePath = pObj->GetTexture()->GetRelativePath();
		SaveWString(strRelativePath, pFile);

		//������Ʈ�� Obstacle type
		int iObsType = (int)pObj->GetObsType();
		fwrite(&iObsType, sizeof(int), 1, pFile);
	}

	//==========Jelly Layer ����============
	
	//������ Jelly Obj ���� ����
	iObjCount = GetLayer(LAYER::JELLY).size();

	fwrite(&iObjCount, sizeof(UINT), 1, pFile);

	//iObjCount��ŭ �ݺ����� ���� ������ ������ ����Ѵ�
	for (size_t i = 0; i < iObjCount; i++)
	{
		CJelly* pObj = dynamic_cast<CJelly*>(GetLayer(LAYER::JELLY)[i]);
		//CTexture* pTex = dynamic_cast<CMapTile*>(pObj)->GetTexture();
		
		// ��ġ
		Vec2 vPos = pObj->GetPos();
		fwrite(&vPos, sizeof(Vec2), 1, pFile);

		//������Ʈ �ؽ����� Ű��
		wstring strKey = pObj->GetTexture()->GetKey();
		SaveWString(strKey, pFile);

		//������Ʈ �ؽ����� ��ΰ�
		wstring strRelativePath = pObj->GetTexture()->GetRelativePath();
		SaveWString(strRelativePath, pFile);

		//������Ʈ�� Item type
		int iItemType = (int)pObj->GetItemType();
		fwrite(&iItemType, sizeof(int), 1, pFile);
	}

	//���� ������ ��ġ�� ��Ʈ���� �ݾ��ش�.
	fclose(pFile);
}

void CEditorLevel::LoadMap()
{
	// open a file name
	OPENFILENAME ofn = {};

	//PathManager���� �⺻ ��θ� �����´�
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

	//���� ������ ��� ��ü ����
	DeleteObject(LAYER::GROUND);
	DeleteObject(LAYER::OBSTACLE);
	DeleteObject(LAYER::JELLY);

	//���������
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"rb");

	//������ Ground Obj ��ü �� �ҷ�����
	UINT iGroundObjCount = 0;
	fread(&iGroundObjCount, sizeof(UINT), 1, pFile);

		//iGroundObjCount��ŭ �ݺ����� ���� ���� �ҷ�����
		for (size_t i = 0; i < iGroundObjCount; i++)
		{
			CGround* pObj = new CGround;

			// ��ġ
			Vec2 vPos;
			fread(&vPos, sizeof(Vec2), 1, pFile);

			// �ؽ�ó �̹��� Key�� �����
			wstring strKey, strRelativePath;
			LoadWString(strKey, pFile);
			LoadWString(strRelativePath, pFile);
			
			//�ҷ��� ���� ���� �� ��ü ����, �ʱ�ȭ
			Instantiate(pObj, vPos, LAYER::GROUND);
			pObj->SetTexture(CResourceManager::GetInst()->LoadTexture(strKey, strRelativePath));
			pObj->init();
		}

		// ������ Obstacle Obj ��ü �� �ҷ�����
		UINT iObstacleObjCount = 0;
		fread(&iObstacleObjCount, sizeof(UINT), 1, pFile);

		//iGroundObjCount��ŭ �ݺ����� ���� ���� �ҷ�����
		for (size_t i = 0; i < iObstacleObjCount; i++)
		{
			CObstacle* pObj = new CObstacle;

			// ��ġ
			Vec2 vPos;
			fread(&vPos, sizeof(Vec2), 1, pFile);

			// �ؽ�ó �̹��� Key�� �����
			wstring strKey, strRelativePath;
			LoadWString(strKey, pFile);
			LoadWString(strRelativePath, pFile);

			//Obstacle Type
			int iObsType = 0;
			fread(&iObsType, sizeof(int), 1, pFile);

			//�ҷ��� ���� ���� �� ��ü ����, �ʱ�ȭ
			Instantiate(pObj, vPos, LAYER::OBSTACLE);
			pObj->SetTexture(CResourceManager::GetInst()->LoadTexture(strKey, strRelativePath));
			pObj->init((OBS_TYPE)iObsType, false);
		}

		// ������ Jelly Obj ��ü �� �ҷ�����
		UINT iJellyObjCount = 0;
		fread(&iJellyObjCount, sizeof(UINT), 1, pFile);

		//iGroundObjCount��ŭ �ݺ����� ���� ���� �ҷ�����
		for (size_t i = 0; i < iJellyObjCount; i++)
		{
			CJelly* pObj = new CJelly;

			// ��ġ
			Vec2 vPos;
			fread(&vPos, sizeof(Vec2), 1, pFile);

			// �ؽ�ó �̹��� Key�� �����
			wstring strKey, strRelativePath;
			LoadWString(strKey, pFile);
			LoadWString(strRelativePath, pFile);

			//Item Type
			int iItemType = 0;
			fread(&iItemType, sizeof(int), 1, pFile);

			//�ҷ��� ���� ���� �� ��ü ����, �ʱ�ȭ
			Instantiate(pObj, vPos, LAYER::JELLY);
			pObj->SetTexture(CResourceManager::GetInst()->LoadTexture(strKey, strRelativePath));
			pObj->init((ITEM_TYPE)iItemType);
		}
	
		fclose(pFile);
	}

	void CEditorLevel::CreatePlayer()
	{
		//�÷��̾� ���� �� �ʱ����
		CPlayer* pPlayer = new CGingerBraveCookie;
		pPlayer->SetPos(Vec2(200.f, 300.f));
		//�÷��̾� ������Ʈ �迭�� �־��ֱ�
		AddObject(pPlayer, LAYER::PLAYER);
		SetPlayer(pPlayer);
		pPlayer->SetHpReduction(0);
		pPlayer->SetSpeed(0);

		CPet* pPet = new CPetChocoDrop;
		SetPet(pPet);
		AddObject(pPet, LAYER::PET);
	}





	


