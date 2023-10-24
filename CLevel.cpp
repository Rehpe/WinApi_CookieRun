#include "pch.h"
#include "CLevel.h"
#include "CObj.h"
#include "CGrid.h"
#include "CPanelUI.h"
#include "CMapTile.h"
#include "CLevelManager.h"
#include "CEditorLevel.h"
#include "CCamera.h"
#include "CEngine.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CGround.h"
#include "CObstacle.h"
#include "CJelly.h"
#include "CPlayer.h"
#include "CKeyManager.h"

CLevel::CLevel()
	: m_iGridXCount(0)
	, m_iGridYCount(0)
	, m_bColliderRender(false)
	, m_bPause(false)
{
}

CLevel::~CLevel()
{
	m_vecVisibleGroundObj.clear();
	m_vecVisibleObstacleObj.clear();
	m_vecVisibleJellyObj.clear();
	DeleteAllObject();
}

void CLevel::tick()
{
	if (CLevelManager::GetInst()->GetCurLevel()->GetPause())
	{
		vector<CObj*>::iterator iter = m_arrLayer[14].begin();
		for (; iter != m_arrLayer[14].end();)
		{
			(*iter)->tick();
			++iter;
		}
		return;
	}


	//기본적인 Level의 tick, 필요시 자식 클래스에서 이 tick을 호출하여 사용한다

	Vec2 vCameraLook = CCamera::GetInst()->GetLook();
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	Vec2 vOffset = Vec2(100.f, 0.f);
	m_vecVisibleGroundObj.clear();
	m_vecVisibleObstacleObj.clear();
	m_vecVisibleJellyObj.clear();
	vector<CObj*> vecVisibleUI = {};
	vector<CGrid*> vecGrid = {};
	vector<CObj*> vecNonGrid = {};


	//일괄적으로 Dead된 객체를 레이어에서 제외한다.
	for (UINT i = 0; i < (UINT)LAYER::END; ++i)
	{
		vector<CObj*>::iterator iter = m_arrLayer[i].begin();

		for (; iter != m_arrLayer[i].end();)
		{
			//Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
			if ((*iter)->IsDead())
			{
				iter = m_arrLayer[i].erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	//맵 최적화를 위해, 특정 레이어(UI 등)를 제외한 오브젝트는 화면 내에 위치할 때에만 tick, render된다.
	UINT iLayer = (int)LAYER::GROUND;
	for (int i = 0; i < m_arrLayer[iLayer].size(); i++)
	{
		CObj* pObj = m_arrLayer[iLayer][i];

		Vec2 vPos = pObj->GetPos();

		if (vCameraLook.x - vResolution.x / 2 - vOffset.x <= vPos.x && vPos.x <= vCameraLook.x + vResolution.x / 2 + vOffset.x)
			m_vecVisibleGroundObj.push_back(pObj);
	}

	iLayer = (int)LAYER::OBSTACLE;
	for (int i = 0; i < m_arrLayer[iLayer].size(); i++)
	{
		CObj* pObj = m_arrLayer[iLayer][i];

		Vec2 vPos = pObj->GetPos();

		if (vCameraLook.x - vResolution.x / 2 - vOffset.x <= vPos.x && vPos.x <= vCameraLook.x + vResolution.x / 2 + vOffset.x)
			m_vecVisibleObstacleObj.push_back(pObj);
	}
	iLayer = (int)LAYER::JELLY;
	for (int i = 0; i < m_arrLayer[iLayer].size(); i++)
	{
		CObj* pObj = m_arrLayer[iLayer][i];

		Vec2 vPos = pObj->GetPos();

		if (vCameraLook.x - vResolution.x / 2 - vOffset.x <= vPos.x && vPos.x <= vCameraLook.x + vResolution.x / 2 + vOffset.x)
			m_vecVisibleJellyObj.push_back(pObj);
	}

	//UI
	iLayer = (int)LAYER::UI;
	for (int i = 0; i < m_arrLayer[iLayer].size(); i++)
	{
		CObj* pObj = m_arrLayer[iLayer][i];

		//그리드인지 확인하고, 그리드인 경우 좌표검사 후 해상도 내의 그리드로 추린다
		CGrid* pGridObj = dynamic_cast<CGrid*>(pObj);

		if (pGridObj)
		{
			Vec2 vPos = pGridObj->GetPos();

			if (vCameraLook.x - vResolution.x / 2 - vOffset.x <= vPos.x && vPos.x <= vCameraLook.x + vResolution.x / 2 + vOffset.x)
				vecVisibleUI.push_back(pGridObj);
		}

		//패널 UI는 무조건 출력하므로 VisibleUI에 추가한다.
		CPanelUI* pPanelObj = dynamic_cast<CPanelUI*>(pObj);
		if (pPanelObj)
			vecVisibleUI.push_back(pPanelObj);
	}

	////해당 레벨이 가지고 있는 오브젝트의 수만큼 반복문을 돌려 각 오브젝트의 tick을 호출한다.
	//for (int i = 0; i < (int)LAYER::END; ++i)
	//{
	//	//땅 레이어
	//	if (i == (int)LAYER::GROUND)
	//	{
	//		vector<CObj*>::iterator iter = m_vecVisibleGroundObj.begin();

	//		for (; iter != m_vecVisibleGroundObj.end();)
	//		{
	//			////Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
	//			//if ((*iter)->IsDead())
	//			//{
	//			//	iter = m_vecVisibleGroundObj.erase(iter);
	//			//}
	//			//else
	//			//{
	//			(*iter)->tick();
	//			++iter;
	//		//}
	//		}
	//	}

	//	//장애물 레이어
	//	else if (i == (int)LAYER::OBSTACLE)
	//	{
	//		vector<CObj*>::iterator iter = m_vecVisibleObstacleObj.begin();

	//		for (; iter != m_vecVisibleObstacleObj.end();)
	//		{
	//			//Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
	//			//if ((*iter)->IsDead())
	//			//{
	//			//	iter = m_vecVisibleObstacleObj.erase(iter);
	//			//}
	//			//else
	//			{
	//				(*iter)->tick();
	//				++iter;
	//			}
	//		}
	//	}
	//	//젤리 레이어
	//	else if (i == (int)LAYER::JELLY)
	//	{
	//		vector<CObj*>::iterator iter = m_vecVisibleJellyObj.begin();

	//		for (; iter != m_vecVisibleJellyObj.end();)
	//		{
	//			////Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
	//			//if ((*iter)->IsDead())
	//			//{
	//			//	iter = m_vecVisibleJellyObj.erase(iter);
	//			//}
	//			//else
	//			{
	//				(*iter)->tick();
	//				++iter;
	//			}
	//		}
	//	}
	//	//그 외 레이어
	//	else
	//	{
	//		vector<CObj*>::iterator iter = m_arrLayer[i].begin();

	//		for (; iter != m_arrLayer[i].end();)
	//		{
	//			////Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
	//			//if ((*iter)->IsDead())
	//			//{
	//			//	iter = m_arrLayer[i].erase(iter);
	//			//}
	//			//else
	//			{
	//				(*iter)->tick();
	//				++iter;
	//			}

	//		}
	//	}
	//}
	//해당 레벨이 가지고 있는 오브젝트의 수만큼 반복문을 돌려 각 오브젝트의 tick을 호출한다.
	for (int i = 0; i < (int)LAYER::END; i++)
	{

		if (i == (int)LAYER::GROUND)
		{
			for (int j = 0; j < m_vecVisibleGroundObj.size(); j++)
			{
				m_vecVisibleGroundObj[j]->tick();
			}
		}

		else if (i == (int)LAYER::OBSTACLE)
		{
			for (int j = 0; j < m_vecVisibleObstacleObj.size(); j++)
			{
				m_vecVisibleObstacleObj[j]->tick();
			}
		}

		else if (i == (int)LAYER::JELLY)
		{
			for (int j = 0; j < m_vecVisibleJellyObj.size(); j++)
			{
				m_vecVisibleJellyObj[j]->tick();
			}
		}
		//else if (i == (int)LAYER::UI)
		//{
		//	for (int j = 0; j < vecVisibleUI.size(); j++)
		//	{

		//		vecVisibleUI[j]->tick();
		//	}
		//}

		else
		{
			vector<CObj*>::iterator iter = m_arrLayer[i].begin();

			for (; iter != m_arrLayer[i].end();)
			{
				(*iter)->tick();
				iter++;
			}
		}
	}
}

void CLevel::final_tick()
{
	if (CLevelManager::GetInst()->GetCurLevel()->GetPause())
	{
		vector<CObj*>::iterator iter = m_arrLayer[14].begin();
		for (; iter != m_arrLayer[14].end();)
		{
			(*iter)->final_tick();
			++iter;
		}
		return;
	}

	//해당 레벨이 가지고 있는 오브젝트의 수만큼 반복문을 돌려 각 오브젝트의 final_tick을 호출한다.
	for (int i = 0; i < (int)LAYER::END; ++i)
	{
		//땅 레이어
		if (i == (int)LAYER::GROUND)
		{
			vector<CObj*>::iterator iter = m_vecVisibleGroundObj.begin();

			for (; iter != m_vecVisibleGroundObj.end();)
			{
				//Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
				if ((*iter)->IsDead())
				{
					iter = m_vecVisibleGroundObj.erase(iter);
				}
				else
				{
					(*iter)->final_tick();
					++iter;
				}
			}
		}

		//장애물 레이어
		else if (i == (int)LAYER::OBSTACLE)
		{
			vector<CObj*>::iterator iter = m_vecVisibleObstacleObj.begin();

			for (; iter != m_vecVisibleObstacleObj.end();)
			{
				//Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
				if ((*iter)->IsDead())
				{
					iter = m_vecVisibleObstacleObj.erase(iter);
				}
				else
				{
					(*iter)->final_tick();
					++iter;
				}
			}
		}
		//젤리 레이어
		else if (i == (int)LAYER::JELLY)
		{
			vector<CObj*>::iterator iter = m_vecVisibleJellyObj.begin();

			for (; iter != m_vecVisibleJellyObj.end();)
			{
				//Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
				if ((*iter)->IsDead())
				{
					iter = m_vecVisibleJellyObj.erase(iter);
				}
				else
				{
					(*iter)->final_tick();
					++iter;
				}
			}
		}
		//그 외 레이어
		else
		{
			vector<CObj*>::iterator iter = m_arrLayer[i].begin();

			for (; iter != m_arrLayer[i].end();)
			{
				//Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
				if ((*iter)->IsDead())
				{
					iter = m_arrLayer[i].erase(iter);
				}
				else
				{
					(*iter)->final_tick();
					++iter;
				}

			}
		}
	}
	////해당 레벨이 가지고 있는 오브젝트의 수만큼 반복문을 돌려 각 오브젝트의 tick을 호출한다.
	//for (int i = 0; i < (int)LAYER::END; i++)
	//{
	//	vector<CObj*>::iterator iter = m_arrLayer[i].begin();

	//	for (; iter != m_arrLayer[i].end();)
	//	{
	//		//Dead 처리된 오브젝트가 있다면 final_tick하지 않고 벡터에서 삭제한다. 
	//		if ((*iter)->IsDead())
	//		{
	//			iter = m_arrLayer[i].erase(iter);
	//		}

	//		(*iter)->final_tick();
	//		iter++;
	//	}
	//}
}

void CLevel::render(HDC _dc)
{
	Vec2 vCameraLook = CCamera::GetInst()->GetLook();
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	Vec2 vOffset = Vec2(100.f, 0.f);
	vector<CObj*> vecVisibleUI = {};

	//UI
	UINT iLayer = (int)LAYER::UI;
	for (int i = 0; i < m_arrLayer[iLayer].size(); i++)
	{
		CObj* pObj = m_arrLayer[iLayer][i];

		//그리드인지 확인하고, 그리드인 경우 좌표검사 후 해상도 내의 그리드로 추린다
		CGrid* pGridObj = dynamic_cast<CGrid*>(pObj);

		if (pGridObj)
		{
			Vec2 vPos = pGridObj->GetPos();

			if (vCameraLook.x - vResolution.x / 2 - vOffset.x <= vPos.x && vPos.x <= vCameraLook.x + vResolution.x / 2 + vOffset.x)
				vecVisibleUI.push_back(pGridObj);
		}
		//패널 UI는 무조건 출력하므로 VisibleUI에 추가한다.
		CPanelUI* pPanelObj = dynamic_cast<CPanelUI*>(pObj);
		if (pPanelObj)
			vecVisibleUI.push_back(pPanelObj);
	}
	//해당 레벨이 가지고 있는 오브젝트의 수만큼 반복문을 돌려 각 오브젝트의 render을 호출한다.
	for (int i = 0; i < (int)LAYER::END; ++i)
	{
		//땅 레이어
		if (i == (int)LAYER::GROUND)
		{
			vector<CObj*>::iterator iter = m_vecVisibleGroundObj.begin();

			for (; iter != m_vecVisibleGroundObj.end();)
			{
				//Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
				if ((*iter)->IsDead())
				{
					iter = m_vecVisibleGroundObj.erase(iter);
				}
				else
				{
					(*iter)->render(_dc);
					++iter;
				}
			}
		}

		//장애물 레이어
		else if (i == (int)LAYER::OBSTACLE)
		{
			vector<CObj*>::iterator iter = m_vecVisibleObstacleObj.begin();

			for (; iter != m_vecVisibleObstacleObj.end();)
			{
				//Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
				if ((*iter)->IsDead())
				{
					iter = m_vecVisibleObstacleObj.erase(iter);
				}
				else
				{
					(*iter)->render(_dc);
					++iter;
				}
			}
		}
		//젤리 레이어
		else if (i == (int)LAYER::JELLY)
		{
			vector<CObj*>::iterator iter = m_vecVisibleJellyObj.begin();

			for (; iter != m_vecVisibleJellyObj.end();)
			{
				//Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
				if ((*iter)->IsDead())
				{
					iter = m_vecVisibleJellyObj.erase(iter);
				}
				else
				{
					(*iter)->render(_dc);
					++iter;
				}
			}
		}
		//그 외 레이어
		else
		{
			vector<CObj*>::iterator iter = m_arrLayer[i].begin();

			for (; iter != m_arrLayer[i].end();)
			{
				//Dead 처리된 오브젝트가 있다면 render하지 않고 벡터에서 삭제한다. 
				if ((*iter)->IsDead())
				{
					iter = m_arrLayer[i].erase(iter);
				}
				else
				{
					(*iter)->render(_dc);
					++iter;
				}

			}
		}
		}
	}


void CLevel::AddObject(CObj* _pObj, LAYER _eLayer)
{
	m_arrLayer[(UINT)_eLayer].push_back(_pObj);
}

void CLevel::CreateGrid(UINT _X, UINT _Y)
{
	//기존에 있던 타일맵을 전부 지워 초기화한다.
	DeleteObject(LAYER::GRID);

	m_iGridXCount = _X;
	m_iGridYCount = _Y;

	//인자로 들어온 가로, 세로 타일 갯수에 맞추어 타일을 배치한다.
	for (UINT iRow = 0; iRow < m_iGridYCount; iRow++)
	{
		for (UINT iCol = 0; iCol < m_iGridXCount; iCol++)
		{
			CGrid* pGrid = new CGrid;
			pGrid->SetPos(Vec2(float(GRID_SIZE * iCol), float(GRID_SIZE * iRow)));
			pGrid->SetDelegate((CEditorLevel*)CLevelManager::GetInst()->GetCurLevel(), (DELEGATE)&CEditorLevel::object_update);
			AddObject(pGrid, LAYER::UI); 
		}
	}
}


void CLevel::SetFocusedUI(CObj* _pUI)
{
	vector<CObj*>& vecUI = m_arrLayer[(UINT)LAYER::UI];

	if (vecUI.back() == _pUI)
		return;

	vector<CObj*>::iterator iter = vecUI.begin();
	for (; iter != vecUI.end(); ++iter)
	{
		if ((*iter) == _pUI)
		{
			vecUI.erase(iter);
			vecUI.push_back(_pUI);
			return;
		}
	}

	// Focued 처리할 UI 가 레벨에 없었다?!?!?!?
	assert(nullptr);
}

void CLevel::DeleteAllObject()
{
	//CLevelManager의 소멸자에서 Level을 삭제하면 CLevel의 소멸자가 호출된다.
	
	//본인(해당 레벨)이 가지고 있는 오브젝트의 수 x 레이어 수만큼 
	//2중 for 반복문을 돌려 각 오브젝트를 삭제한다.

	for (int i = 0; i < (int)LAYER::END; i++)
	{
		for (int j = 0; j < m_arrLayer[i].size(); j++)
		{
			//이미 Dead처리된 오브젝트는 건너뛴다.
			if (m_arrLayer[i][j]->IsDead())
				continue;

			DEL(m_arrLayer[i][j]);
		}
		//동적 할당이 해제된 벡터의 주소값들을 완전히 비운다.
		m_arrLayer[i].clear();
	}
}

void CLevel::DeleteObject(LAYER _eLayer)
{
	//레이어 벡터 내에 있는 모든 오브젝트들을 삭제하고, 벡터를 완전히 비운다.
	for (size_t i = 0; i < m_arrLayer[(UINT)_eLayer].size(); i++)
	{
		DEL(m_arrLayer[(UINT)_eLayer][i]);
	}

	m_arrLayer[(UINT)_eLayer].clear();
}

void CLevel::LevelDestroy()
{
	for (int i = 0; i < (int)LAYER::END; i++)
	{
		if (i == (int)LAYER::PLAYER)
			continue;

		if (i == (int)LAYER::PET)
			continue;
		
		if (i == (int)LAYER::UI)
			continue;

		if (i == (int)LAYER::PLAYER_SKILL)
			continue;

		else
		{
			for (int j = 0; j < m_arrLayer[i].size(); j++)
			{
				//이미 Dead처리된 오브젝트는 건너뛴다.
				if (m_arrLayer[i][j]->IsDead())
					continue;

				DEL(m_arrLayer[i][j]);
			}
			//동적 할당이 해제된 벡터의 주소값들을 완전히 비운다.
			m_arrLayer[i].clear();
		} 
		
	}
}

void CLevel::LoadMap(const wstring& _strRelativePath)
{
	//PathManager에서 기본 경로를 가져온다
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();

	//인자로 들어온 상대경로를 붙여 최종 경로를 완성한다.
	strFilePath += _strRelativePath;

	//기존 생성된 모든 객체 삭제
	DeleteObject(LAYER::GROUND);
	DeleteObject(LAYER::OBSTACLE);
	DeleteObject(LAYER::JELLY);

	//파일입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

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

void CLevel::Pause()
{
	CLevelManager::GetInst()->GetCurLevel()->m_bPause = true;

	//일시정지 패널
	m_pPausePanel = new CPanelUI;
	CTexture* pTex = CResourceManager::GetInst()->LoadTexture(L"PausePanel", L"texture\\UI\\PausePanel.png");
	m_pPausePanel->SetIdleTex(pTex);
	m_pPausePanel->SetPos(Vec2(0.f, 0.f));

	//다시하기 버튼
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	CButtonUI* pBreakPauseBtn = new CButtonUI;
	pTex = CResourceManager::GetInst()->LoadTexture(L"Puase_BreakPauseBtn", L"texture\\UI\\Puase_BreakPauseBtn.png");
	pBreakPauseBtn->SetIdleTex(pTex);
	pBreakPauseBtn->SetPos(vResolution / 2.f + Vec2(-150.f, -50.f));
	pBreakPauseBtn->SetDelegate(this, (DELEGATE)&CLevel::BreakPause);

	m_pPausePanel->AddChildUI(pBreakPauseBtn);

	//그만하기 버튼
	CButtonUI* pStopGameBtn = new CButtonUI;
	pTex = CResourceManager::GetInst()->LoadTexture(L"Puase_StopGameBtn", L"texture\\UI\\Puase_StopGameBtn.png");
	pStopGameBtn->SetIdleTex(pTex);
	pStopGameBtn->SetPos(vResolution / 2.f + Vec2(-150.f, 70.f));
	pStopGameBtn->SetDelegate(this, (DELEGATE)&CLevel::StopGame);

	m_pPausePanel->AddChildUI(pStopGameBtn);

	CLevelManager::GetInst()->GetCurLevel()->AddObject(m_pPausePanel, LAYER::UI);
}

void CLevel::BreakPause()
{
	CLevelManager::GetInst()->GetCurLevel()->m_bPause = false;

	m_pPausePanel->SetDead();
}

void CLevel::StopGame()
{
	CLevelManager::GetInst()->GetCurLevel()->m_bPause = false;
	CLevelManager::GetInst()->GetCurLevel()->GetPlayer()->SetPlayerDead(true);
	ChangeLevel(LEVEL_TYPE::RESULT);
}
