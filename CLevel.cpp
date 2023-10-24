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


	//�⺻���� Level�� tick, �ʿ�� �ڽ� Ŭ�������� �� tick�� ȣ���Ͽ� ����Ѵ�

	Vec2 vCameraLook = CCamera::GetInst()->GetLook();
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	Vec2 vOffset = Vec2(100.f, 0.f);
	m_vecVisibleGroundObj.clear();
	m_vecVisibleObstacleObj.clear();
	m_vecVisibleJellyObj.clear();
	vector<CObj*> vecVisibleUI = {};
	vector<CGrid*> vecGrid = {};
	vector<CObj*> vecNonGrid = {};


	//�ϰ������� Dead�� ��ü�� ���̾�� �����Ѵ�.
	for (UINT i = 0; i < (UINT)LAYER::END; ++i)
	{
		vector<CObj*>::iterator iter = m_arrLayer[i].begin();

		for (; iter != m_arrLayer[i].end();)
		{
			//Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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

	//�� ����ȭ�� ����, Ư�� ���̾�(UI ��)�� ������ ������Ʈ�� ȭ�� ���� ��ġ�� ������ tick, render�ȴ�.
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

		//�׸������� Ȯ���ϰ�, �׸����� ��� ��ǥ�˻� �� �ػ� ���� �׸���� �߸���
		CGrid* pGridObj = dynamic_cast<CGrid*>(pObj);

		if (pGridObj)
		{
			Vec2 vPos = pGridObj->GetPos();

			if (vCameraLook.x - vResolution.x / 2 - vOffset.x <= vPos.x && vPos.x <= vCameraLook.x + vResolution.x / 2 + vOffset.x)
				vecVisibleUI.push_back(pGridObj);
		}

		//�г� UI�� ������ ����ϹǷ� VisibleUI�� �߰��Ѵ�.
		CPanelUI* pPanelObj = dynamic_cast<CPanelUI*>(pObj);
		if (pPanelObj)
			vecVisibleUI.push_back(pPanelObj);
	}

	////�ش� ������ ������ �ִ� ������Ʈ�� ����ŭ �ݺ����� ���� �� ������Ʈ�� tick�� ȣ���Ѵ�.
	//for (int i = 0; i < (int)LAYER::END; ++i)
	//{
	//	//�� ���̾�
	//	if (i == (int)LAYER::GROUND)
	//	{
	//		vector<CObj*>::iterator iter = m_vecVisibleGroundObj.begin();

	//		for (; iter != m_vecVisibleGroundObj.end();)
	//		{
	//			////Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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

	//	//��ֹ� ���̾�
	//	else if (i == (int)LAYER::OBSTACLE)
	//	{
	//		vector<CObj*>::iterator iter = m_vecVisibleObstacleObj.begin();

	//		for (; iter != m_vecVisibleObstacleObj.end();)
	//		{
	//			//Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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
	//	//���� ���̾�
	//	else if (i == (int)LAYER::JELLY)
	//	{
	//		vector<CObj*>::iterator iter = m_vecVisibleJellyObj.begin();

	//		for (; iter != m_vecVisibleJellyObj.end();)
	//		{
	//			////Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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
	//	//�� �� ���̾�
	//	else
	//	{
	//		vector<CObj*>::iterator iter = m_arrLayer[i].begin();

	//		for (; iter != m_arrLayer[i].end();)
	//		{
	//			////Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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
	//�ش� ������ ������ �ִ� ������Ʈ�� ����ŭ �ݺ����� ���� �� ������Ʈ�� tick�� ȣ���Ѵ�.
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

	//�ش� ������ ������ �ִ� ������Ʈ�� ����ŭ �ݺ����� ���� �� ������Ʈ�� final_tick�� ȣ���Ѵ�.
	for (int i = 0; i < (int)LAYER::END; ++i)
	{
		//�� ���̾�
		if (i == (int)LAYER::GROUND)
		{
			vector<CObj*>::iterator iter = m_vecVisibleGroundObj.begin();

			for (; iter != m_vecVisibleGroundObj.end();)
			{
				//Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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

		//��ֹ� ���̾�
		else if (i == (int)LAYER::OBSTACLE)
		{
			vector<CObj*>::iterator iter = m_vecVisibleObstacleObj.begin();

			for (; iter != m_vecVisibleObstacleObj.end();)
			{
				//Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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
		//���� ���̾�
		else if (i == (int)LAYER::JELLY)
		{
			vector<CObj*>::iterator iter = m_vecVisibleJellyObj.begin();

			for (; iter != m_vecVisibleJellyObj.end();)
			{
				//Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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
		//�� �� ���̾�
		else
		{
			vector<CObj*>::iterator iter = m_arrLayer[i].begin();

			for (; iter != m_arrLayer[i].end();)
			{
				//Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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
	////�ش� ������ ������ �ִ� ������Ʈ�� ����ŭ �ݺ����� ���� �� ������Ʈ�� tick�� ȣ���Ѵ�.
	//for (int i = 0; i < (int)LAYER::END; i++)
	//{
	//	vector<CObj*>::iterator iter = m_arrLayer[i].begin();

	//	for (; iter != m_arrLayer[i].end();)
	//	{
	//		//Dead ó���� ������Ʈ�� �ִٸ� final_tick���� �ʰ� ���Ϳ��� �����Ѵ�. 
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

		//�׸������� Ȯ���ϰ�, �׸����� ��� ��ǥ�˻� �� �ػ� ���� �׸���� �߸���
		CGrid* pGridObj = dynamic_cast<CGrid*>(pObj);

		if (pGridObj)
		{
			Vec2 vPos = pGridObj->GetPos();

			if (vCameraLook.x - vResolution.x / 2 - vOffset.x <= vPos.x && vPos.x <= vCameraLook.x + vResolution.x / 2 + vOffset.x)
				vecVisibleUI.push_back(pGridObj);
		}
		//�г� UI�� ������ ����ϹǷ� VisibleUI�� �߰��Ѵ�.
		CPanelUI* pPanelObj = dynamic_cast<CPanelUI*>(pObj);
		if (pPanelObj)
			vecVisibleUI.push_back(pPanelObj);
	}
	//�ش� ������ ������ �ִ� ������Ʈ�� ����ŭ �ݺ����� ���� �� ������Ʈ�� render�� ȣ���Ѵ�.
	for (int i = 0; i < (int)LAYER::END; ++i)
	{
		//�� ���̾�
		if (i == (int)LAYER::GROUND)
		{
			vector<CObj*>::iterator iter = m_vecVisibleGroundObj.begin();

			for (; iter != m_vecVisibleGroundObj.end();)
			{
				//Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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

		//��ֹ� ���̾�
		else if (i == (int)LAYER::OBSTACLE)
		{
			vector<CObj*>::iterator iter = m_vecVisibleObstacleObj.begin();

			for (; iter != m_vecVisibleObstacleObj.end();)
			{
				//Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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
		//���� ���̾�
		else if (i == (int)LAYER::JELLY)
		{
			vector<CObj*>::iterator iter = m_vecVisibleJellyObj.begin();

			for (; iter != m_vecVisibleJellyObj.end();)
			{
				//Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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
		//�� �� ���̾�
		else
		{
			vector<CObj*>::iterator iter = m_arrLayer[i].begin();

			for (; iter != m_arrLayer[i].end();)
			{
				//Dead ó���� ������Ʈ�� �ִٸ� render���� �ʰ� ���Ϳ��� �����Ѵ�. 
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
	//������ �ִ� Ÿ�ϸ��� ���� ���� �ʱ�ȭ�Ѵ�.
	DeleteObject(LAYER::GRID);

	m_iGridXCount = _X;
	m_iGridYCount = _Y;

	//���ڷ� ���� ����, ���� Ÿ�� ������ ���߾� Ÿ���� ��ġ�Ѵ�.
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

	// Focued ó���� UI �� ������ ������?!?!?!?
	assert(nullptr);
}

void CLevel::DeleteAllObject()
{
	//CLevelManager�� �Ҹ��ڿ��� Level�� �����ϸ� CLevel�� �Ҹ��ڰ� ȣ��ȴ�.
	
	//����(�ش� ����)�� ������ �ִ� ������Ʈ�� �� x ���̾� ����ŭ 
	//2�� for �ݺ����� ���� �� ������Ʈ�� �����Ѵ�.

	for (int i = 0; i < (int)LAYER::END; i++)
	{
		for (int j = 0; j < m_arrLayer[i].size(); j++)
		{
			//�̹� Deadó���� ������Ʈ�� �ǳʶڴ�.
			if (m_arrLayer[i][j]->IsDead())
				continue;

			DEL(m_arrLayer[i][j]);
		}
		//���� �Ҵ��� ������ ������ �ּҰ����� ������ ����.
		m_arrLayer[i].clear();
	}
}

void CLevel::DeleteObject(LAYER _eLayer)
{
	//���̾� ���� ���� �ִ� ��� ������Ʈ���� �����ϰ�, ���͸� ������ ����.
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
				//�̹� Deadó���� ������Ʈ�� �ǳʶڴ�.
				if (m_arrLayer[i][j]->IsDead())
					continue;

				DEL(m_arrLayer[i][j]);
			}
			//���� �Ҵ��� ������ ������ �ּҰ����� ������ ����.
			m_arrLayer[i].clear();
		} 
		
	}
}

void CLevel::LoadMap(const wstring& _strRelativePath)
{
	//PathManager���� �⺻ ��θ� �����´�
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();

	//���ڷ� ���� ����θ� �ٿ� ���� ��θ� �ϼ��Ѵ�.
	strFilePath += _strRelativePath;

	//���� ������ ��� ��ü ����
	DeleteObject(LAYER::GROUND);
	DeleteObject(LAYER::OBSTACLE);
	DeleteObject(LAYER::JELLY);

	//���������
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

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

void CLevel::Pause()
{
	CLevelManager::GetInst()->GetCurLevel()->m_bPause = true;

	//�Ͻ����� �г�
	m_pPausePanel = new CPanelUI;
	CTexture* pTex = CResourceManager::GetInst()->LoadTexture(L"PausePanel", L"texture\\UI\\PausePanel.png");
	m_pPausePanel->SetIdleTex(pTex);
	m_pPausePanel->SetPos(Vec2(0.f, 0.f));

	//�ٽ��ϱ� ��ư
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	CButtonUI* pBreakPauseBtn = new CButtonUI;
	pTex = CResourceManager::GetInst()->LoadTexture(L"Puase_BreakPauseBtn", L"texture\\UI\\Puase_BreakPauseBtn.png");
	pBreakPauseBtn->SetIdleTex(pTex);
	pBreakPauseBtn->SetPos(vResolution / 2.f + Vec2(-150.f, -50.f));
	pBreakPauseBtn->SetDelegate(this, (DELEGATE)&CLevel::BreakPause);

	m_pPausePanel->AddChildUI(pBreakPauseBtn);

	//�׸��ϱ� ��ư
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
