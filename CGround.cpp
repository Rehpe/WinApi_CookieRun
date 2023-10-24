#include "pch.h"
#include "CGround.h"
#include "CCollider.h"
#include "CTexture.h"
#include "CResourceManager.h"
#include "CCamera.h"
#include "CPlayer.h"
#include "CRigidbody.h"

CGround::CGround()
{
	m_pTexture = CResourceManager::GetInst()->LoadTexture(L"ground1", L"texture\\map\\mapTile\\ground1.png");
	CMapTile::init();
}

CGround::~CGround()
{
}

void CGround::render(HDC _dc)
{
	Vec2 vPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	//AlphaBlend 설정
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f);

	AlphaBlend(_dc
		, (int)(vPos.x - m_pTexture->GetWidth() / 2)
		, (int)(vPos.y - m_pTexture->GetHeight() / 2)
		, m_pTexture->GetWidth()
		, m_pTexture->GetHeight()
		, m_pTexture->GetDC()
		, 0, 0
		, m_pTexture->GetWidth()
		, m_pTexture->GetHeight()
		, tBlend);

	CObj::render(_dc);
}

void CGround::BeginOverlap(CCollider* _pOther)
{
	//다이나믹캐스트를 이용하여 충돌한 오브젝트가 player 클래스인지 확인할 수 있다
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetOwner());
	if (pPlayer == nullptr)
		return;

	pPlayer->GetRigidbody()->SetGround(true);

	Vec2 vPlayerColPos = pPlayer->GetCollider()->GetFinalPos();
	Vec2 vPlayerColScale = pPlayer->GetCollider()->GetFinalScale();

	Vec2 vGroundPos = GetCollider()->GetFinalPos();
	Vec2 vGroundScale = GetCollider()->GetFinalScale();

	float fLength = fabsf(vPlayerColPos.y - vGroundPos.y);
	float fValue = (vPlayerColScale.y / 2.f + vGroundScale.y / 2.f) - fLength;

	Vec2 vPlayerPos = pPlayer->GetPos();
	vPlayerPos.y -= fValue;
	pPlayer->SetPos(Vec2(vPlayerPos.x, vPlayerPos.y));
}

void CGround::OnOverlap(CCollider* _pOther)
{
	//다이나믹캐스트를 이용하여 충돌한 오브젝트가 player 클래스인지 확인할 수 있다
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetOwner());
	if (pPlayer == nullptr)
		return;

	Vec2 vPlayerColPos = pPlayer->GetCollider()->GetFinalPos();
	Vec2 vPlayerColScale = pPlayer->GetCollider()->GetFinalScale();

	Vec2 vGroundPos = GetCollider()->GetFinalPos();
	Vec2 vGroundScale = GetCollider()->GetFinalScale();

	float fLength = fabsf(vPlayerColPos.y - vGroundPos.y);
	float fValue = (vPlayerColScale.y / 2.f + vGroundScale.y / 2.f) - fLength;

	Vec2 vPlayerPos = pPlayer->GetPos();
	vPlayerPos.y -= fValue;
	pPlayer->SetPos(Vec2(vPlayerPos.x, vPlayerPos.y));
}

void CGround::EndOverlap(CCollider* _pOther)
{
	//다이나믹캐스트를 이용하여 충돌한 오브젝트가 player 클래스인지 확인할 수 있다
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_pOther->GetOwner());
	if (pPlayer == nullptr)
		return;

	pPlayer->GetRigidbody()->SetGround(false);
}
