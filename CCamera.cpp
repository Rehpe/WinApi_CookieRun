#include "pch.h"
#include "CCamera.h"
#include "CEngine.h"
#include "CKeyManager.h"
#include "CResourceManager.h"
#include "CTimeManager.h"
#include "CTexture.h"

CCamera::CCamera()
	: m_pBlindTex(nullptr)
	, m_fRatio(0.f)
	, m_bCamShake(false)
	, m_fShakeAccTime(0.f)
	, m_fShakeMaxTime(0.f)
	, m_fShakeSpeed(0.f)
	, m_fRange(0.f)
	, m_fCamSpeed(500.f)
{
	//현재 게임의 해상도를 받아와 해상도와 동일한 사이즈의 검정 텍스쳐 생성
	POINT ptResolution = CEngine::GetInst()->GetResolution();
	m_pBlindTex = CResourceManager::GetInst()->CreateTexture(L"BlindTex", ptResolution.x, ptResolution.y);
}

CCamera::~CCamera()
{
}

void CCamera::tick()
{
	// 카메라 좌표 이동 (WASD)
	if (IsPressed(KEY::W))
		m_vLook.y -= m_fCamSpeed * DT;
	if (IsPressed(KEY::S))
		m_vLook.y += m_fCamSpeed * DT;
	if (IsPressed(KEY::A))
		m_vLook.x -= m_fCamSpeed * DT;
	if (IsPressed(KEY::D))
		m_vLook.x += m_fCamSpeed * DT;

	//현재 윈도우의 해상도를 구해와 해상도의 중심 위치값을 계산한다
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	vResolution /= 2.f;

	//현재 카메라가 보고 있는 위치와 해상도의 중심 위치의 차이값 저장
	m_vDiff = m_vLook + m_vOffset - vResolution;

	//카메라 효과
	CameraEffect();

	//카메라 흔들림
	CameraShake();
}

void CCamera::render(HDC _dc)
{
	//카메라 효과(Fade in/out) 구현을 위해 알파블렌드로 렌더링한다.

	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = 0;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f * m_fRatio); //m_fRatio 값만큼 투명화된다

	//쓸모없는 alphablend 연산을 줄이기 위해 m_fRatio가 0이라면 도중에 return시켜버린다.
	if (m_fRatio < 0.0001f)
		return;

	AlphaBlend(_dc
		, 0
		, 0
		, m_pBlindTex->GetWidth()
		, m_pBlindTex->GetHeight()
		, m_pBlindTex->GetDC()
		, 0, 0
		, m_pBlindTex->GetWidth()
		, m_pBlindTex->GetHeight()
		, tBlend);

}

Vec2 CCamera::GetRenderPos(Vec2 _vRealPos)
{
	//실제 오브젝트의 위치에서 카메라 위치에 따른 좌표 차이값을 뺀, 물체가 렌더링되어야하는 좌표
	return _vRealPos - m_vDiff;
}

Vec2 CCamera::GetRealPos(Vec2 _vRenderPos)
{
	//GetRenderPos와는 반대로, 마우스 등으로 오브젝트가 위치한 좌표 등을 찍을 때
	//렌더링 된 곳이 아닌 실제 좌표를 찍을 수 있도록 반대 연산을 해야한다.
	return _vRenderPos + m_vDiff;
}

void CCamera::FadeOut(float _fLapse)
{
	tCamEffect effect = {};

	effect.m_eCurEffect = CAMERA_EFFECT::FADE_OUT;
	effect.m_fAccTime = 0.f;
	effect.m_fMaxTime = _fLapse;

	m_CamEffectList.push_back(effect);
}

void CCamera::FadeIn(float _fLapse)
{
	tCamEffect effect = {};

	effect.m_eCurEffect = CAMERA_EFFECT::FADE_IN;
	effect.m_fAccTime = 0.f;
	effect.m_fMaxTime = _fLapse;

	m_CamEffectList.push_back(effect);
}

void CCamera::Dim(float _fLapse)
{
	tCamEffect effect = {};

	effect.m_eCurEffect = CAMERA_EFFECT::DIM;
	effect.m_fAccTime = 0.f;
	effect.m_fMaxTime = _fLapse;

	m_CamEffectList.push_back(effect);
}

void CCamera::CameraShake(float _fRange, float _fSpeed, float _fLapse)
{
	m_fShakeAccTime = 0.f;
	m_fShakeMaxTime = _fLapse;
	m_fRange = _fRange;
	m_fShakeSpeed = _fSpeed;
	m_fShakeDir = 1.f;
	m_bCamShake = true;
}

void CCamera::CameraEffect()
{
	//카메라 효과 리스트에 요소가 있을 경우에만 함수가 진행된다.
	if (m_CamEffectList.empty())
		return;

	//카메라 효과 리스트가 차있다면 순서대로 효과를 실행한다.
	tCamEffect& effect = m_CamEffectList.front();

	//누적 시간을 받는다
	effect.m_fAccTime += DT;

	//누적 시간이 효과 지속시간을 넘어가면
	if (effect.m_fAccTime >= effect.m_fMaxTime)
	{
		//리스트에서 pop-front로 제거해버리고 리턴
		m_CamEffectList.pop_front();
		return;
	}

	//비율은 누적시간 / 전체 지속시간
	m_fRatio = effect.m_fAccTime / effect.m_fMaxTime;

	//FadeIn이라면 어두운 상태에서 점점 밝아져야하므로 fRatio가 1부터 점점 작아져야함
	if (effect.m_eCurEffect == CAMERA_EFFECT::FADE_IN)
		m_fRatio = 1 - m_fRatio;

	//DIM이라면 ratio는 변하지 않는다
	else if (effect.m_eCurEffect == CAMERA_EFFECT::DIM)
		m_fRatio = 0.5;
}

void CCamera::CameraShake()
{
	//m_bCamShake가 true 일때만 함수 진행
	if (!m_bCamShake)
		return;

	//누적 시간을 받는다
	m_fShakeAccTime += DT;

	//누적 시간이 흔들림 최대 지속시간을 넘어간다면 
	if (m_fShakeAccTime >= m_fShakeMaxTime)
	{
		//m_bcamShake를 끄고 offset을 0으로 초기화한 뒤 리턴한다.
		m_bCamShake = false;
		m_vOffset = Vec2(0.f, 0.f);
		return;
	}

	m_vOffset.y += DT * m_fShakeSpeed * m_fShakeDir;

	//설정한 범위보다 오프셋의 절댓값이 커지면
	if (fabsf(m_vOffset.y) > m_fRange)
	{
		//offset값을 다시 Range값으로 초기화한다.
		m_vOffset.y = m_fRange * m_fShakeDir;

		//흔들림 방향을 반대로 바꾸어준다.
		m_fShakeDir *= -1;
	}
}
