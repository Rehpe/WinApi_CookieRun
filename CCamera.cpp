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
	//���� ������ �ػ󵵸� �޾ƿ� �ػ󵵿� ������ �������� ���� �ؽ��� ����
	POINT ptResolution = CEngine::GetInst()->GetResolution();
	m_pBlindTex = CResourceManager::GetInst()->CreateTexture(L"BlindTex", ptResolution.x, ptResolution.y);
}

CCamera::~CCamera()
{
}

void CCamera::tick()
{
	// ī�޶� ��ǥ �̵� (WASD)
	if (IsPressed(KEY::W))
		m_vLook.y -= m_fCamSpeed * DT;
	if (IsPressed(KEY::S))
		m_vLook.y += m_fCamSpeed * DT;
	if (IsPressed(KEY::A))
		m_vLook.x -= m_fCamSpeed * DT;
	if (IsPressed(KEY::D))
		m_vLook.x += m_fCamSpeed * DT;

	//���� �������� �ػ󵵸� ���ؿ� �ػ��� �߽� ��ġ���� ����Ѵ�
	Vec2 vResolution = CEngine::GetInst()->GetResolution();
	vResolution /= 2.f;

	//���� ī�޶� ���� �ִ� ��ġ�� �ػ��� �߽� ��ġ�� ���̰� ����
	m_vDiff = m_vLook + m_vOffset - vResolution;

	//ī�޶� ȿ��
	CameraEffect();

	//ī�޶� ��鸲
	CameraShake();
}

void CCamera::render(HDC _dc)
{
	//ī�޶� ȿ��(Fade in/out) ������ ���� ���ĺ���� �������Ѵ�.

	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = 0;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f * m_fRatio); //m_fRatio ����ŭ ����ȭ�ȴ�

	//������� alphablend ������ ���̱� ���� m_fRatio�� 0�̶�� ���߿� return���ѹ�����.
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
	//���� ������Ʈ�� ��ġ���� ī�޶� ��ġ�� ���� ��ǥ ���̰��� ��, ��ü�� �������Ǿ���ϴ� ��ǥ
	return _vRealPos - m_vDiff;
}

Vec2 CCamera::GetRealPos(Vec2 _vRenderPos)
{
	//GetRenderPos�ʹ� �ݴ��, ���콺 ������ ������Ʈ�� ��ġ�� ��ǥ ���� ���� ��
	//������ �� ���� �ƴ� ���� ��ǥ�� ���� �� �ֵ��� �ݴ� ������ �ؾ��Ѵ�.
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
	//ī�޶� ȿ�� ����Ʈ�� ��Ұ� ���� ��쿡�� �Լ��� ����ȴ�.
	if (m_CamEffectList.empty())
		return;

	//ī�޶� ȿ�� ����Ʈ�� ���ִٸ� ������� ȿ���� �����Ѵ�.
	tCamEffect& effect = m_CamEffectList.front();

	//���� �ð��� �޴´�
	effect.m_fAccTime += DT;

	//���� �ð��� ȿ�� ���ӽð��� �Ѿ��
	if (effect.m_fAccTime >= effect.m_fMaxTime)
	{
		//����Ʈ���� pop-front�� �����ع����� ����
		m_CamEffectList.pop_front();
		return;
	}

	//������ �����ð� / ��ü ���ӽð�
	m_fRatio = effect.m_fAccTime / effect.m_fMaxTime;

	//FadeIn�̶�� ��ο� ���¿��� ���� ��������ϹǷ� fRatio�� 1���� ���� �۾�������
	if (effect.m_eCurEffect == CAMERA_EFFECT::FADE_IN)
		m_fRatio = 1 - m_fRatio;

	//DIM�̶�� ratio�� ������ �ʴ´�
	else if (effect.m_eCurEffect == CAMERA_EFFECT::DIM)
		m_fRatio = 0.5;
}

void CCamera::CameraShake()
{
	//m_bCamShake�� true �϶��� �Լ� ����
	if (!m_bCamShake)
		return;

	//���� �ð��� �޴´�
	m_fShakeAccTime += DT;

	//���� �ð��� ��鸲 �ִ� ���ӽð��� �Ѿ�ٸ� 
	if (m_fShakeAccTime >= m_fShakeMaxTime)
	{
		//m_bcamShake�� ���� offset�� 0���� �ʱ�ȭ�� �� �����Ѵ�.
		m_bCamShake = false;
		m_vOffset = Vec2(0.f, 0.f);
		return;
	}

	m_vOffset.y += DT * m_fShakeSpeed * m_fShakeDir;

	//������ �������� �������� ������ Ŀ����
	if (fabsf(m_vOffset.y) > m_fRange)
	{
		//offset���� �ٽ� Range������ �ʱ�ȭ�Ѵ�.
		m_vOffset.y = m_fRange * m_fShakeDir;

		//��鸲 ������ �ݴ�� �ٲپ��ش�.
		m_fShakeDir *= -1;
	}
}
