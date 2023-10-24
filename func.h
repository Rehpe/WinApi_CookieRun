#pragma once
class CObj;
class CState;
class CFSM;

//전역으로 사용할 함수를 정의해두는 헤더

//포인터가 유효한지 알려주는 함수(유효성 체크)
bool IsValid(CObj*& _pTarget);

//저장할 값은 바뀌면 안되니까 const 레퍼런스
void SaveWString(const wstring& _str, FILE* _pFile);

//로드해온 값을 알맞게 수정해야하니까 일반 레퍼런스
void LoadWString(wstring& _str, FILE* _pFile);

//오브젝트의 생성
void Instantiate(CObj* _pNewObj, Vec2 _vPos, LAYER _eLayer);

//포화함수(어떤 값을 0과 1 사이로 고정하는 함수)
void Saturate(float& _float);

//레벨을 바꾸는 함수
void ChangeLevel(LEVEL_TYPE _eNextLevel);

//충돌관계 원상복귀(플레이어 / 땅 / 장애물 / 젤리)
void ResetCollision();

//버튼 클릭음 재생
void BtnClickSound();


