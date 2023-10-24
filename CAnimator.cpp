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
	//_other.m_mapAnim의 객체들을 반복문을 돌며 깊은 복사하여 복사할 맵에 넣어준다.
	map<wstring, CAnimation*>::const_iterator iter = _other.m_mapAnim.begin();
	for (; iter != _other.m_mapAnim.end(); iter++)
	{
		//애니메이션 포인터 클론
		CAnimation* pAnim = iter->second->Clone();
		//애니메이션의 소유 애니메이터를 자신으로 설정
		pAnim->m_pAnimator = this;
		//애니메이션 프레임 정보 등을 리셋
		pAnim->Reset();
		//복사한 애니메이션 맵에 새로 만든 애니메이션 삽입
		m_mapAnim.insert(make_pair(iter->first, pAnim));
	}

	//복사가 끝나면 원본 애니메이터와 같은 애니메이션을 재생시킨다
	if (m_pCurAnim != nullptr)
		Play(_other.m_pCurAnim->GetName(), m_bRepeat);
}

CAnimator::~CAnimator()
{
	//소유한 애니메이션 맵을 순회하며 모든 애니메이션을 삭제한다
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
	//현재 지정된 애니메이션이 없다면 리턴
	if (m_pCurAnim == nullptr)
		return;

	//현재 애니메이션이 재생 종료된 상태이고, 반복재생이 true라면
	if (m_pCurAnim->IsFinish() && m_bRepeat)
	{
		//애니메이션을 초기화시킨다(계속 첫 프레임부터 반복 재생된다)
		m_pCurAnim->Reset();
	}

	//현재 애니메이션의 tick 호출
	m_pCurAnim->tick();
}

void CAnimator::render(HDC _dc)
{
	//현재 지정된 애니메이션이 없다면 리턴
	if (m_pCurAnim == nullptr)
		return;

	//현재 애니메이션의 render 호출
	m_pCurAnim->render(_dc);
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	//애니메이션 맵에서 키값으로 애니메이션을 검색한다
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_strName);

	//못찾으면 nullptr 반환
	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

CAnimation* CAnimator::LoadAnimation(const wstring& _strRelativePath)
{
	//본인(애니메이터)를 주인으로 하는 애니메이션을 생성한 후
	CAnimation* pAnim = new CAnimation(this);

	//인자의 경로 위치에서 불러온 애니메이션을 받아온다.
	pAnim->Load(_strRelativePath);

	//애니메이션 맵에 키값으로 저장한다. 
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
	
	return pAnim;
}

void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pAtlas, Vec2 _vLeftTop, Vec2 _vSize, Vec2 _vOffset, int _iMaxFrmCount, float _fDuration)
{
	//만들기 전, 해당 이름으로 이미 만들어진 애니메이션이 있는지 검색해본다.
	CAnimation* pAnim = FindAnimation(_strName);

	//nullptr이 아니라면 같은 이름의(미리 만들어진) 애니메이션이 이미 있다는 뜻으로 에러처리
	assert(!pAnim);

	//nullptr이라면 애니메이션을 새로 만든다.
	pAnim = new CAnimation(this);

	//새로 만든 애니메이션을 들어온 인자들로 초기화한 후 맵에 넣어준다.
	pAnim->init(_strName, _pAtlas, _vLeftTop, _vSize, _vOffset, _iMaxFrmCount, _fDuration);
	m_mapAnim.insert(make_pair(_strName, pAnim));
}


void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
	//애니메이션을 실행시키기 전, 맵에서 해당 애니메이션을 찾는다.
	CAnimation* pAnim = FindAnimation(_strName);

	//해당 애니메이션을 찾지 못했다면 메세지박스로 알림을 주고 return
	if (pAnim == nullptr)
	{
		MessageBox(nullptr, L"해당 Animation 없음", L"Animation 재생 오류", MB_OK);
		return;
	}

	//검색되어 반환된 애니메이션을 현재 애니메이션으로 지정한다.
	m_pCurAnim = pAnim;
	m_pCurAnim->Reset();
	m_bRepeat = _bRepeat;
}


