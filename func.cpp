#include "pch.h"
#include "func.h"

#include "CObj.h"
#include "CEventManager.h"
#include "CCollisionManager.h"
#include "CResourceManager.h"
#include "CSound.h"

bool IsValid(CObj*& _pTarget)
{
	if (_pTarget == nullptr)
	{
		return false;
	}

	else if (_pTarget->IsDead())
	{
		_pTarget = nullptr;
		return false;
	}

	return true;
}

void SaveWString(const wstring& _str, FILE* _pFile)
{
	//문자열의 길이 저장
	size_t iLen = _str.length();
	fwrite(&iLen, sizeof(size_t), 1, _pFile);

	//위에 저장한 문자열의 길이만큼 문자열 저장
	fwrite(_str.c_str(), sizeof(wchar_t), iLen, _pFile);
}

void LoadWString(wstring& _str, FILE* _pFile)
{
	//문자열의 길이 불러오기
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, _pFile);

	//경로 문자열의 길이는 OS 구조상 255글자를 넘을 수 없으므로, 
	//null문자 포함한 256짜리 버퍼를 만들어 한번에 받아온다.
	wchar_t szBuff[256] = {};
	fread(szBuff, sizeof(wchar_t), iLen, _pFile);
	_str = szBuff;
}

void Instantiate(CObj* _pNewObj, Vec2 _vPos, LAYER _eLayer)
{
	_pNewObj->SetPos(_vPos);

	//해당 미사일 생성 과정을 이벤트 매니저에게 전달한다.
	tEvent _evn = {};
	_evn.eType = EVENT_TYPE::CREATE_OBJECT;
	_evn.wParam = (DWORD_PTR)_pNewObj;
	_evn.lParam = (DWORD_PTR)_eLayer;
	CEventManager::GetInst()->AddEvent(_evn);
}

void Saturate(float& _float)
{
	if (_float > 1.f)
		_float = 1.f;
	if (_float < 0.f)
		_float = 0.f;
}


void ChangeLevel(LEVEL_TYPE _eNextLevel)
{
	tEvent _evn = {};
	_evn.eType = EVENT_TYPE::LEVEL_CHANGE;
	_evn.wParam = (DWORD_PTR)_eNextLevel;
	CEventManager::GetInst()->AddEvent(_evn);
}

void ResetCollision()
{
	CCollisionManager::GetInst()->Clear();
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::GROUND);
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::JELLY);
	CCollisionManager::GetInst()->LayerCheck(LAYER::PLAYER, LAYER::OBSTACLE);
}

void BtnClickSound()
{
	CSound* pSound = CResourceManager::GetInst()->LoadSound(L"BtnClick", L"sound\\UI\\UI_BtnClick.wav");
	pSound->Play(false);
}
