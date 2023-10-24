#pragma once

//������ â�� �������� ��ǥ���� �������ִ� Ŭ����

class CTexture;


struct tCamEffect
{
	CAMERA_EFFECT	m_eCurEffect;	//ī�޶� ȿ�� ����
	float			m_fAccTime;		//���� �ð�
	float			m_fMaxTime;		//ī�޶� ȿ�� �ִ� ���� �ð�
};

class CCamera
{
	SINGLETON(CCamera);

private:
	Vec2	m_vLook;		// ī�޶� ���� �ִ� ���� ��ǥ
	Vec2	m_vDiff;		// ����/������ ��ǥ���� ���̰�
	float	m_fCamSpeed;	// ī�޶� ������ �ӵ�

	//ī�޶� ȿ��(Fade in/Fade out)
	CTexture*			m_pBlindTex;		//ȭ�� ��ȯ ȿ���� ���� �ؽ���
	float				m_fRatio;
	list<tCamEffect>	m_CamEffectList;	//ī�޶� ȿ���� ���ʴ�� ó���� ����Ʈ

	//ī�޶� ��鸲 ȿ��
	Vec2				m_vOffset;			//Look�� ���� ���� ī�޶� ������ ������ �� �ִ�
	float				m_fShakeAccTime;	//ī�޶� ��鸲 ���� ���� �ð�
	float				m_fShakeMaxTime;	//ī�޶� ��鸲 �ִ� ���� �ð�
	float				m_fRange;			//ī�޶� ��鸲 ����
	float				m_fShakeDir;		//ī�޶� ��鸲 ����
	float				m_fShakeSpeed;		//ī�޶� ��鸲 �ӵ�
	bool				m_bCamShake;		//ī�޶� ��鸲 ������ ����

public:
	void tick();
	void render(HDC _dc);

public:
	Vec2 GetRenderPos(Vec2 _vRealPos);	//���� ��ǥ �����ϸ� ������ ��ǥ ��ȯ���ִ� �Լ�
	Vec2 GetRealPos(Vec2 _vRenderPos);	//������ ��ǥ �����ϸ� ���� ��ǥ ��ȯ���ִ� �Լ�

public:
	Vec2 GetLook() { return m_vLook; }
	void SetLook(Vec2 _vLook) { m_vLook = _vLook; }
	void FadeOut(float _fLapse);
	void FadeIn(float _fLapse);
	void Dim(float _fLapse);

	void CameraShake(float _fRange, float _fSpeed, float _fLapse);

	bool IsCameraEffectDone() { return m_CamEffectList.empty(); }

private:
	//ī�޶� ȿ�� �Լ����� ���������� ���� ����� �Լ�
	void CameraEffect();
	void CameraShake();
};

