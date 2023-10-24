#include "pch.h"
#include "CCollider.h"
#include "CEngine.h"
#include "CObj.h"
#include "CCamera.h"
#include "CPlayer.h"

//�θ� ��ӹ��� �ڽ� Ŭ������ �θ��� ������� �ʱ�ȭ�� �θ��� �⺻ �����ڸ� ���� �ϴµ�,
//�θ��� �⺻ �����ڰ� �����Ƿ� �θ��� ������� �����ڿ� ���ڸ� �־ ȣ���Ų��.
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
	//�浹ü�� ��ü���� �̵��� ���� ���Ŀ� ��ġ�� �����ؾ��ϹǷ�, ����� final_tick���� �����Ѵ�.

	//��ø ���� ������ ���� ����ó��
	assert(!(m_iOverlapCount < 0));
}

void CCollider::final_tick()
{
	//�浹ü�� ���� ��ġ���� �����Ѵ�.

	//�浹ü�� ���� ��ü�� ��ġ���� �����´�.
	Vec2 vOwnerPos = GetOwner()->GetPos();
	Vec2 vOwnerScale = GetOwner()->GetScale();
	
	//offset�� ���� �浹ü�� ���� ��ġ�� �����Ѵ�.
	m_vFinalPos = vOwnerPos + m_vOffsetPos;
	m_vFinalScale = (vOwnerScale * m_fSizeRatio) + m_vOffsetScale;
}

void CCollider::render(HDC _dc)
{
	//�������� ���� �浹ü�� ������ �������Ѵ�.

	//�浹�ϰ� ���� ��� collider�� ���� ������, �ƴ� ��� �ʷ� ������ �׸���.
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

	//DC�� ���� ��� �귯�ø� ���� ������ �͵�� ��ü�Ѵ�.
	HPEN hDefaultPen = (HPEN)SelectObject(_dc, hPen);
	HBRUSH hDefaultBrush = (HBRUSH)SelectObject(_dc, hNullBrush);
	 
	//ī�޶� Ŭ������ �������� ��ǥ�� ��ȯ�Ѵ�.
	m_vFinalPos = CCamera::GetInst()->GetRenderPos(m_vFinalPos);

	//�簢���� �׸���.
		Rectangle(_dc, (int)(m_vFinalPos.x) - (m_vFinalScale.x / 2)
			, (int)(m_vFinalPos.y) - (m_vFinalScale.y / 2)
			, (int)(m_vFinalPos.x + m_vFinalScale.x / 2)
			, (int)(m_vFinalPos.y + m_vFinalScale.y / 2));

	//DC�� GDI ������Ʈ���� ������ ��� �귯���� �ǵ�����.
	SelectObject(_dc, hDefaultPen);
	SelectObject(_dc, hDefaultBrush);
}

void CCollider::BeginOverlap(CCollider* _pOther)
{
	//������ ī��Ʈ�� +1 ���ְ�, ������ ��ü�� BeginOverlap�� ȣ�����ش�.
	m_iOverlapCount++;

	GetOwner()->BeginOverlap(_pOther);
}

void CCollider::OnOverlap(CCollider* _pOther)
{
	//������ ��ü���� ��� �浹���̶�� OnOverlap �Լ��� ȣ���Ų��.
	GetOwner()->OnOverlap(_pOther);
}

void CCollider::EndOverlap(CCollider* _pOther)
{
	//������ ī��Ʈ�� -1 ���ְ�, ������ ��ü�� EndOverlap�� ȣ���Ų��.
	m_iOverlapCount--;
	GetOwner()->EndOverlap(_pOther);
}
