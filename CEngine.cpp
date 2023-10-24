#include "pch.h"
#include "CEngine.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CLevelManager.h"
#include "CCamera.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CEventManager.h"
#include "CCollisionManager.h"
#include "CUIManager.h"
#include "CSoundManager.h"

CEngine::CEngine()
	: m_hMainWnd(nullptr)
	, m_hDC(nullptr)
	, m_ptResolution{}
	, m_pMemTex(nullptr)
	, m_arrPen{}
{
}

CEngine::~CEngine()
{
	//������ ���� �˾Ƽ� ���α׷��� ����� �� ����ǳ�
	//DC, Ÿ�̸� ���� Ŀ�� ������Ʈ�� ���� ���� �Ҹ��ڿ��� ��������־���Ѵ�.
	ReleaseDC(m_hMainWnd, m_hDC);

	//������ ���� �����Ѵ�.
	for (UINT i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}
}

void CEngine::init(HWND _hwnd, UINT _iWidth, UINT _iHeight)
{
	//���ڷ� ���� ���� �����츦 ������� m_hMainWnd�� ����
	m_hMainWnd = _hwnd;

	//���ڷ� ���� �ػ� ������ ������� m_ptResolution�� ����
	m_ptResolution.x = _iWidth;
	m_ptResolution.y = _iHeight;

	//HDC �ʱ�ȭ(Handle Device Context)
	m_hDC = GetDC(m_hMainWnd);

	//������ ũ�� ����
	ChangeWindowSize(_iWidth, _iHeight);

	//���� ����ϴ� ���� ��� �귯���� �̸� �����д�.
	CreatePenBrush();

	//Manager Ŭ���� �ʱ�ȭ
	CPathManager::GetInst()->init();
	CTimeManager::GetInst()->init();
	CKeyManager::GetInst()->init();
	CSoundManager::GetInst()->init();
	CLevelManager::GetInst()->init();
}

void CEngine::progress()
{	
	//������ �����Ӹ��� tick�� render�� ���� ��� �Ŵ������� ����Ѵ�.

	//���� ����
	tick();

	//ȭ�� ������ ����
	render();

	//Render���� ���� ��, ����ó���� �ʿ��� �̺�Ʈ ó��
	CEventManager::GetInst()->tick();
}

void CEngine::tick()
{
	//=====�� �Ŵ����鿡�� ������ �°� tick ���=====

	//��� ������ DT�� ������� �̷�������ϱ� ������, TimeManager�� tick�� ���� ���� ȣ���ؾ��Ѵ�.
	CTimeManager::GetInst()->tick();

	//DT�� ���� ���� Ű ���¸� üũ�ϱ� ���� KeyManager�� tick�� ȣ���Ѵ�.
	CKeyManager::GetInst()->tick();

	//Camera
	CCamera::GetInst()->tick();

	//�� ��, ������ �� �����Ӹ��� �����Ŵ����� tick�� ȣ���Ѵ�.
	CLevelManager::GetInst()->tick();

	//������ ������ �ִ� ������Ʈ���� ����/�������� ������, �浹 ����
	CCollisionManager::GetInst()->tick();

	//UI �̺�Ʈ üũ
	CUIManager::GetInst()->tick();
}

void CEngine::render()
{
	//ȭ�� ���� �ܻ��� ���ֱ� ���� ���۸� ����� �� ������ ���ۿ� �׷��� ȭ������ �����Ѵ�
	
	//1.���� �ؽ��� ȭ���� ������ ���������
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	//�� �� ������ ���� �ؽ��Ŀ� �������Ѵ�. 
	CLevelManager::GetInst()->render(m_pMemTex->GetDC());

	//ī�޶� ����Ʈ�� �ִٸ� �̶� ������
	CCamera::GetInst()->render(m_pMemTex->GetDC());

	//�������� ������, ���� �ؽ��Ŀ��� ���� ������ ��Ʈ������ �������� ȭ���� �����Ѵ�
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
		m_pMemTex->GetDC(), 0, 0, SRCCOPY);

	//TimeManager -> Ÿ��Ʋâ�� FPS �����
	CTimeManager::GetInst()->render();

}

void CEngine::ChangeWindowSize(UINT _iWidth, UINT _iHeight)
{
	//���ڷ� ���� ���� ��������� ����
	m_ptResolution.x = _iWidth;
	m_ptResolution.y = _iHeight;

	//���⿡ ���� width�� height ���� ������ Ÿ��Ʋâ�� �޴����� ũ�⸦ ������ ������,
	//��Ȯ�ϰ� Ÿ��Ʋ�ٸ� ������ �ػ󵵸� ���߰� �ʹٸ� AdjustWindowRect �Լ��� ����Ѵ�.

	//��,��,��,�� �� ���� ��ǥ���� ���� ����ü RECT
	RECT rt = { 0, 0, m_ptResolution.x, m_ptResolution.y };

	//���� �������� �޴� ������ �����´�
	HMENU hMenu = GetMenu(m_hMainWnd);

	//�޴��� �ִ� ��������
	if (hMenu != nullptr)
	{
		//���ڷ� RECT�� �ּҰ�, ������ ��Ÿ��, �޴��� ���� 
		//���⼭ &rt�� INOUT ���ڷ�, �� �Լ��� &rt�� ���� �����Ͽ�
		//������ ��Ÿ���� ����� ��Ȯ�� �ػ� ����� �ٽ� &rt�� ����� ���̴�.
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	}
	else
	{
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	}

	//���� &rt�� ���ϴ� ������ ���� ��ƿ�����, SetWindowPos �Լ��� �����Ѵ�.
	SetWindowPos(m_hMainWnd, nullptr, 0, 0,
		rt.right - rt.left, rt.bottom - rt.top, 0);

	//����ۿ� �ؽ��İ� ���ٸ� ����
	if (m_pMemTex == nullptr)
	{
		//�ܻ� ���� ȭ�� �� ��Ʈ�� ����
		m_pMemTex = CResourceManager::GetInst()->CreateTexture(L"BackBuffer", m_ptResolution.x, m_ptResolution.y);
	}
	//�ִٸ� ������ ����۸� ���������Ѵ�.
	else
	{
		m_pMemTex->Resize(m_ptResolution.x, m_ptResolution.y);
	}
}

void CEngine::CreatePenBrush()
{
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}
