#include "pch.h"
#include "CAnimator.h"
#include "CAnimation.h"

CAnimator::CAnimator(CObj* _pOwner)
	: CComponent(_pOwner)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
	, m_fRatio(1)
	, m_fSizeRatio(1)
	, m_vOffset(Vec2(0.f, 0.f))
{
}

CAnimator::CAnimator(const CAnimator& _other)
	:CComponent(_other)
	, m_pCurAnim(_other.m_pCurAnim)
	, m_bRepeat(_other.m_bRepeat)
{
	//_other.m_mapAnim�� ��ü���� �ݺ����� ���� ���� �����Ͽ� ������ �ʿ� �־��ش�.
	map<wstring, CAnimation*>::const_iterator iter = _other.m_mapAnim.begin();
	for (; iter != _other.m_mapAnim.end(); iter++)
	{
		//�ִϸ��̼� ������ Ŭ��
		CAnimation* pAnim = iter->second->Clone();
		//�ִϸ��̼��� ���� �ִϸ����͸� �ڽ����� ����
		pAnim->m_pAnimator = this;
		//�ִϸ��̼� ������ ���� ���� ����
		pAnim->Reset();
		//������ �ִϸ��̼� �ʿ� ���� ���� �ִϸ��̼� ����
		m_mapAnim.insert(make_pair(iter->first, pAnim));
	}

	//���簡 ������ ���� �ִϸ����Ϳ� ���� �ִϸ��̼��� �����Ų��
	if (m_pCurAnim != nullptr)
		Play(_other.m_pCurAnim->GetName(), m_bRepeat);
}

CAnimator::~CAnimator()
{
	//������ �ִϸ��̼� ���� ��ȸ�ϸ� ��� �ִϸ��̼��� �����Ѵ�
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); iter++)
	{
		delete iter->second;
	}
	m_mapAnim.clear();
}

void CAnimator::tick()
{

}

void CAnimator::final_tick()
{
	//���� ������ �ִϸ��̼��� ���ٸ� ����
	if (m_pCurAnim == nullptr)
		return;

	//���� �ִϸ��̼��� ��� ����� �����̰�, �ݺ������ true���
	if (m_pCurAnim->IsFinish() && m_bRepeat)
	{
		//�ִϸ��̼��� �ʱ�ȭ��Ų��(��� ù �����Ӻ��� �ݺ� ����ȴ�)
		m_pCurAnim->Reset();
	}

	//���� �ִϸ��̼��� tick ȣ��
	m_pCurAnim->tick();
}

void CAnimator::render(HDC _dc)
{
	//���� ������ �ִϸ��̼��� ���ٸ� ����
	if (m_pCurAnim == nullptr)
		return;

	//���� �ִϸ��̼��� render ȣ��
	m_pCurAnim->render(_dc);
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	//�ִϸ��̼� �ʿ��� Ű������ �ִϸ��̼��� �˻��Ѵ�
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_strName);

	//��ã���� nullptr ��ȯ
	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

CAnimation* CAnimator::LoadAnimation(const wstring& _strRelativePath)
{
	//����(�ִϸ�����)�� �������� �ϴ� �ִϸ��̼��� ������ ��
	CAnimation* pAnim = new CAnimation(this);

	//������ ��� ��ġ���� �ҷ��� �ִϸ��̼��� �޾ƿ´�.
	pAnim->Load(_strRelativePath);

	//�ִϸ��̼� �ʿ� Ű������ �����Ѵ�. 
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
	
	return pAnim;
}

void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pAtlas, Vec2 _vLeftTop, Vec2 _vSize, Vec2 _vOffset, int _iMaxFrmCount, float _fDuration)
{
	//����� ��, �ش� �̸����� �̹� ������� �ִϸ��̼��� �ִ��� �˻��غ���.
	CAnimation* pAnim = FindAnimation(_strName);

	//nullptr�� �ƴ϶�� ���� �̸���(�̸� �������) �ִϸ��̼��� �̹� �ִٴ� ������ ����ó��
	assert(!pAnim);

	//nullptr�̶�� �ִϸ��̼��� ���� �����.
	pAnim = new CAnimation(this);

	//���� ���� �ִϸ��̼��� ���� ���ڵ�� �ʱ�ȭ�� �� �ʿ� �־��ش�.
	pAnim->init(_strName, _pAtlas, _vLeftTop, _vSize, _vOffset, _iMaxFrmCount, _fDuration);
	m_mapAnim.insert(make_pair(_strName, pAnim));
}


void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
	//�ִϸ��̼��� �����Ű�� ��, �ʿ��� �ش� �ִϸ��̼��� ã�´�.
	CAnimation* pAnim = FindAnimation(_strName);

	//�ش� �ִϸ��̼��� ã�� ���ߴٸ� �޼����ڽ��� �˸��� �ְ� return
	if (pAnim == nullptr)
	{
		MessageBox(nullptr, L"�ش� Animation ����", L"Animation ��� ����", MB_OK);
		return;
	}

	//�˻��Ǿ� ��ȯ�� �ִϸ��̼��� ���� �ִϸ��̼����� �����Ѵ�.
	m_pCurAnim = pAnim;
	m_pCurAnim->Reset();
	m_bRepeat = _bRepeat;
}


