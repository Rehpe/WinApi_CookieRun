#include "pch.h"
#include "CMapTileBtn.h"
#include "CTexture.h"
#include "CLevelManager.h"
#include "CLevel.h"
#include "CPreview.h"
#include "CEditorLevel.h"

CMapTileBtn::CMapTileBtn()
	:m_eClass(CLASS_TYPE::NONE)
{
}

CMapTileBtn::~CMapTileBtn()
{
}

void CMapTileBtn::render(HDC _dc)
{
	CTexture* pTargetTex = GetIdleTex();

	if (!pTargetTex)
	{
		//�ؽ��İ� ���ٸ� BTN_SIZE ũ�⸸ŭ�� �簢�� ��ư ���

		Rectangle(_dc
			, int(GetFinalPos().x)
			, int(GetFinalPos().y)
			, int(GetFinalPos().x + BTN_SIZE)
			, int(GetFinalPos().y + BTN_SIZE));

		CUI::render(_dc);
		return;
	}

	Vec2 vPos = GetFinalPos();
	//AlphaBlend ����
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f);

	AlphaBlend(_dc
		, (int)(vPos.x)
		, (int)(vPos.y)
		, BTN_SIZE
		, BTN_SIZE
		, GetIdleTex()->GetDC()
		, 0, 0
		, GetIdleTex()->GetWidth()
		, GetIdleTex()->GetHeight()
		, tBlend);

	render_childUI(_dc);
}

void CMapTileBtn::selectObject()
{
	CEditorLevel* pCurLevel = dynamic_cast<CEditorLevel*>(CLevelManager::GetInst()->GetCurLevel());

	//if (pCurLevel != LEVEL_TYPE::EDITOR)
	//	return;
	
	//Preview ��ü �����

	//�̹� ������ ������ ��ü�� ���� ��� ���� �� ���� ����
	if (pCurLevel->GetPreview() != nullptr)
		pCurLevel->GetPreview()->SetDead();

	CPreview* pPreview = new CPreview;
	
	//�����ͷ����� ���� ������� ���� �� AddObject
	pCurLevel->SetPreview(pPreview);
	pCurLevel->AddObject(pPreview, LAYER::PREVIEW);
	
	//������ ��ü�� ���� �ؽ��� ����
	pPreview->SetPreviewTexture(GetIdleTex());

	//�����ͷ����� ������ ����(Ŭ���� Ÿ��, ���̾� Ÿ��) �� ���� ���� ����
	pCurLevel->SetClassType(m_eClass);
}
