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
		//텍스쳐가 없다면 BTN_SIZE 크기만큼의 사각형 버튼 출력

		Rectangle(_dc
			, int(GetFinalPos().x)
			, int(GetFinalPos().y)
			, int(GetFinalPos().x + BTN_SIZE)
			, int(GetFinalPos().y + BTN_SIZE));

		CUI::render(_dc);
		return;
	}

	Vec2 vPos = GetFinalPos();
	//AlphaBlend 설정
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
	
	//Preview 객체 만들기

	//이미 생성된 프리뷰 객체가 있을 경우 삭제 후 새로 생성
	if (pCurLevel->GetPreview() != nullptr)
		pCurLevel->GetPreview()->SetDead();

	CPreview* pPreview = new CPreview;
	
	//에디터레벨의 현재 프리뷰로 지정 후 AddObject
	pCurLevel->SetPreview(pPreview);
	pCurLevel->AddObject(pPreview, LAYER::PREVIEW);
	
	//프리뷰 객체에 본인 텍스쳐 대입
	pPreview->SetPreviewTexture(GetIdleTex());

	//에디터레벨의 생성용 변수(클래스 타입, 레이어 타입) 에 본인 정보 대입
	pCurLevel->SetClassType(m_eClass);
}
