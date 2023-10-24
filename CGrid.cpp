#include "pch.h"
#include "CGrid.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CKeyManager.h"

CGrid::CGrid()
	//:m_pInst(nullptr)
	//,m_pDelegateFunc(nullptr)
{
	SetScale(Vec2((float)GRID_SIZE, (float)GRID_SIZE));
}

CGrid::~CGrid()
{
}
//
//void CGrid::tick()
//{
//	Vec2 vPos = GetPos();
//
//	if (IsTap(KEY::D))
//	{
//		vPos += Vec2(200.f, 0.f);
//		SetPos(vPos);
//	}
//	if (IsTap(KEY::A))
//	{
//		vPos += Vec2(-200.f, 0.f);
//		SetPos(vPos);
//	}
//}

void CGrid::render(HDC _dc)
{
	
	//CTexture* pTargetTex = GetIdleTex();

	HBRUSH	hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH hDefaultBrush = (HBRUSH)SelectObject(_dc, hNullBrush);

	//본인의 위치좌표를 렌더링좌표로 변환
	Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());

	////인자로 들어온 가로, 세로 타일 갯수에 맞추어 타일을 배치한다.
	//for (UINT iRow = 0; iRow < 16; iRow++)
	//{
	//	for (UINT iCol = 0; iCol < 500; iCol++)
	//	{
	//		Rectangle(_dc
	//			, int(vPos.x + GRID_SIZE * iCol)
	//			, int(vPos.y + GRID_SIZE * iRow  )
	//			, int(GRID_SIZE)
	//			, int(GRID_SIZE));
	//	}
	//}
	//SelectObject(_dc, hDefaultBrush);

	Rectangle(_dc
		, int(vPos.x)
		, int(vPos.y)
		, int(vPos.x + GRID_SIZE)
		, int(vPos.y + GRID_SIZE));	

	SelectObject(_dc, hDefaultBrush);
}

void CGrid::MouseLbtnClicked()
{
	CButtonUI::MouseLbtnClicked();
}

