#pragma once

//윈도우 창에 보여지는 좌표값을 관리해주는 클래스

class CTexture;


struct tCamEffect
{
	CAMERA_EFFECT	m_eCurEffect;	//카메라 효과 종류
	float			m_fAccTime;		//누적 시간
	float			m_fMaxTime;		//카메라 효과 최대 지속 시간
};

class CCamera
{
	SINGLETON(CCamera);

private:
	Vec2	m_vLook;		// 카메라가 보고 있는 현재 좌표
	Vec2	m_vDiff;		// 실제/렌더링 좌표간의 차이값
	float	m_fCamSpeed;	// 카메라 움직임 속도

	//카메라 효과(Fade in/Fade out)
	CTexture*			m_pBlindTex;		//화면 전환 효과용 검은 텍스쳐
	float				m_fRatio;
	list<tCamEffect>	m_CamEffectList;	//카메라 효과를 차례대로 처리할 리스트

	//카메라 흔들림 효과
	Vec2				m_vOffset;			//Look값 변경 없이 카메라 초점을 수정할 수 있다
	float				m_fShakeAccTime;	//카메라 흔들림 진행 누적 시간
	float				m_fShakeMaxTime;	//카메라 흔들림 최대 지속 시간
	float				m_fRange;			//카메라 흔들림 범위
	float				m_fShakeDir;		//카메라 흔들림 방향
	float				m_fShakeSpeed;		//카메라 흔들림 속도
	bool				m_bCamShake;		//카메라 흔들림 진행중 여부

public:
	void tick();
	void render(HDC _dc);

public:
	Vec2 GetRenderPos(Vec2 _vRealPos);	//실제 좌표 대입하면 렌더링 좌표 반환해주는 함수
	Vec2 GetRealPos(Vec2 _vRenderPos);	//렌더링 좌표 대입하면 실제 좌표 반환해주는 함수

public:
	Vec2 GetLook() { return m_vLook; }
	void SetLook(Vec2 _vLook) { m_vLook = _vLook; }
	void FadeOut(float _fLapse);
	void FadeIn(float _fLapse);
	void Dim(float _fLapse);

	void CameraShake(float _fRange, float _fSpeed, float _fLapse);

	bool IsCameraEffectDone() { return m_CamEffectList.empty(); }

private:
	//카메라 효과 함수에서 내부적으로 쓰일 비공개 함수
	void CameraEffect();
	void CameraShake();
};

