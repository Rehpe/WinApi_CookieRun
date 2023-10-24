#pragma once

//게임 전체를 관장하는 엔진 클래스
class CTexture;

class CEngine
{
	SINGLETON(CEngine);

private:
	HWND		m_hMainWnd;						//메인 윈도우 핸들
	HDC			m_hDC;							//메인 DC
	POINT		m_ptResolution;					//메인 윈도우의 해상도 정보

	HPEN		m_arrPen[(UINT)PEN_TYPE::END];	//자주 쓸 펜을 담아둘 배열

	CTexture*	m_pMemTex;						//잔상 없는 화면용 버퍼 텍스처

public:
	void init(HWND _hwnd, UINT _iWidth, UINT _iHeight);
	void progress();

public:
	void tick();	//오브젝트 갱신
	void render();	//오브젝트 렌더링

public:
	HWND	GetMainWnd() { return m_hMainWnd; }
	HDC		GetMainDC() { return m_hDC; }
	HPEN	GetPen(PEN_TYPE _type) { return m_arrPen[(UINT)_type]; }
	POINT	GetResolution() { return m_ptResolution; }

public:
	void ChangeWindowSize(UINT _width, UINT _iHeight);

	void CreatePenBrush();


};

