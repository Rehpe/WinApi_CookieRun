#include "pch.h"
#include "CTile.h"
#include "CCamera.h"
#include "CTexture.h"

CTile::CTile()
	: m_pAtlas(nullptr)
	, m_iImgIdx(-1)
{
}

CTile::~CTile()
{
}

void CTile::tick()
{
}

void CTile::render(HDC _dc)
{
	//타일 아틀라스 이미지가 설정되어있지 않거나, 인덱스가 설정되어 있지 않다면 return
	//if (m_iImgIdx == -1 || m_pAtlas == nullptr)
		//return;

	//본인의 위치좌표를 렌더링좌표로 변환
	Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());

	//int iMaxCol = m_pAtlas->GetWidth() / TILE_SIZE;

	//타일의 인덱스 번호에 따라 아틀라스 내의 몇행 몇열에 위치하고 있는지 계산한다.
	//int iCurRow = m_iImgIdx / iMaxCol;
	//int iCurCol = m_iImgIdx % iMaxCol;

	//Rectangle(_dc
	//	, int(vPos.x)
	//	, int(vPos.y)
	//	, int(vPos.x + TILE_SIZE)
	//	, int(vPos.y + TILE_SIZE));

	/*BitBlt(_dc
		, int(vPos.x)
		, int(vPos.y)
		, TILE_SIZE
		, TILE_SIZE
		, m_pAtlas->GetDC()
		, iCurCol * TILE_SIZE
		, iCurRow * TILE_SIZE
		, SRCCOPY);*/
}

void CTile::Save(FILE* _pFile)
{
}

void CTile::Load(FILE* _pFile)
{
}

void CTile::SetImgIdx(int _imgidx)
{
}

void CTile::AddImgIdx()
{
}
