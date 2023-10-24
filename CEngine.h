#pragma once

//���� ��ü�� �����ϴ� ���� Ŭ����
class CTexture;

class CEngine
{
	SINGLETON(CEngine);

private:
	HWND		m_hMainWnd;						//���� ������ �ڵ�
	HDC			m_hDC;							//���� DC
	POINT		m_ptResolution;					//���� �������� �ػ� ����

	HPEN		m_arrPen[(UINT)PEN_TYPE::END];	//���� �� ���� ��Ƶ� �迭

	CTexture*	m_pMemTex;						//�ܻ� ���� ȭ��� ���� �ؽ�ó

public:
	void init(HWND _hwnd, UINT _iWidth, UINT _iHeight);
	void progress();

public:
	void tick();	//������Ʈ ����
	void render();	//������Ʈ ������

public:
	HWND	GetMainWnd() { return m_hMainWnd; }
	HDC		GetMainDC() { return m_hDC; }
	HPEN	GetPen(PEN_TYPE _type) { return m_arrPen[(UINT)_type]; }
	POINT	GetResolution() { return m_ptResolution; }

public:
	void ChangeWindowSize(UINT _width, UINT _iHeight);

	void CreatePenBrush();


};

