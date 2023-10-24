#include "pch.h"
#include "CCollider.h"
#include "CEngine.h"
#include "CObj.h"
#include "CCamera.h"
#include "CPlayer.h"

//부모를 상속받은 자식 클래스는 부모의 멤버변수 초기화를 부모의 기본 생성자를 통해 하는데,
//부모의 기본 생성자가 없으므로 부모의 만들어준 생성자에 인자를 넣어를 호출시킨다.
CCollider::CCollider(CObj* _pOwner)
	: CComponent(_pOwner)
	, m_iOverlapCount(0)
	, m_fSizeRatio(1)
{

}

CCollider::CCollider(const CCollider& _other)
	: CComponent(nullptr)
	, m_vOffsetPos(_other.m_vOffsetPos)
	, m_vScale(_other.m_vScale)
	, m_iOverlapCount(0)
	, m_fSizeRatio(1)
{
}

CCollider::~CCollider()
{
}

void CCollider::tick()
{
	//충돌체는 객체들의 이동이 끝난 이후에 위치를 결정해야하므로, 기능을 final_tick에서 구현한다.

	//중첩 수가 음수인 경우는 에러처리
	assert(!(m_iOverlapCount < 0));
}

void CCollider::final_tick()
{
	//충돌체의 최종 위치값을 결정한다.

	//충돌체를 가진 객체의 위치값을 가져온다.
	Vec2 vOwnerPos = GetOwner()->GetPos();
	Vec2 vOwnerScale = GetOwner()->GetScale();
	
	//offset을 더해 충돌체의 최종 위치를 결정한다.
	m_vFinalPos = vOwnerPos + m_vOffsetPos;
	m_vFinalScale = (vOwnerScale * m_fSizeRatio) + m_vOffsetScale;
}

void CCollider::render(HDC _dc)
{
	//가독성을 위해 충돌체의 범위를 렌더링한다.

	//충돌하고 있을 경우 collider을 빨간 펜으로, 아닐 경우 초록 펜으로 그린다.
	HPEN hPen = nullptr;

	if (m_iOverlapCount > 0)
	{
		hPen = CEngine::GetInst()->GetPen(PEN_TYPE::RED);
	}
	else
	{
		hPen = CEngine::GetInst()->GetPen(PEN_TYPE::GREEN);
	}

	HBRUSH	hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	//DC의 기존 펜과 브러시를 새로 가져온 것들로 대체한다.
	HPEN hDefaultPen = (HPEN)SelectObject(_dc, hPen);
	HBRUSH hDefaultBrush = (HBRUSH)SelectObject(_dc, hNullBrush);
	 
	//카메라 클래스의 렌더링용 좌표로 변환한다.
	m_vFinalPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);

	//사각형을 그린다.
		Rectangle(_dc, (int)(m_vFinalPos.x) - (m_vFinalScale.x / 2)
			, (int)(m_vFinalPos.y) - (m_vFinalScale.y / 2)
			, (int)(m_vFinalPos.x + m_vFinalScale.x / 2)
			, (int)(m_vFinalPos.y + m_vFinalScale.y / 2));

	//DC의 GDI 오브젝트들을 기존의 펜과 브러쉬로 되돌린다.
	SelectObject(_dc, hDefaultPen);
	SelectObject(_dc, hDefaultBrush);
}

void CCollider::BeginOverlap(CCollider* _pOther)
{
	//오버랩 카운트를 +1 해주고, 소유한 객체의 BeginOverlap을 호출해준다.
	m_iOverlapCount++;

	GetOwner()->BeginOverlap(_pOther);
}

void CCollider::OnOverlap(CCollider* _pOther)
{
	//소유한 객체에게 계속 충돌중이라는 OnOverlap 함수를 호출시킨다.
	GetOwner()->OnOverlap(_pOther);
}

void CCollider::EndOverlap(CCollider* _pOther)
{
	//오버랩 카운트를 -1 해주고, 소유한 객체의 EndOverlap을 호출시킨다.
	m_iOverlapCount--;
	GetOwner()->EndOverlap(_pOther);
}
