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
	//Ÿ�� ��Ʋ�� �̹����� �����Ǿ����� �ʰų�, �ε����� �����Ǿ� ���� �ʴٸ� return
	//if (m_iImgIdx == -1 || m_pAtlas == nullptr)
		//return;

	//������ ��ġ��ǥ�� ��������ǥ�� ��ȯ
	Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());

	//int iMaxCol = m_pAtlas->GetWidth() / TILE_SIZE;

	//Ÿ���� �ε��� ��ȣ�� ���� ��Ʋ�� ���� ���� ��� ��ġ�ϰ� �ִ��� ����Ѵ�.
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
