#include "pch.h"
#include "CTexture.h"
#include "CEngine.h"

CTexture::CTexture()
	: m_hBit(nullptr)
	, m_hDC(nullptr)
	, m_tBitmapInfo{}
{
}

CTexture::~CTexture()
{
	//������ ��Ʈ�ʰ� DC�� �����Ѵ�
	DeleteObject(m_hBit);
	DeleteDC(m_hDC);
}


int CTexture::Load(const wstring& _strFilePath)
{
	//GDI+ �ʼ� �ڵ�
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// image.png ������ �̿��Ͽ� ����� ������ �ҷ��� Image ��ü�� �����մϴ�.
	Image* image = Image::FromFile(_strFilePath.c_str(),false);

	//�ҷ��� image�� Clone�Ͽ� Bitmap���Ϸ� ��ȯ ��, HBITMAP���Ϸ� ��ȯ�մϴ�.
	Bitmap* pBitmap = static_cast<Bitmap*>(image);//(image->Clone());
	Status status = pBitmap->GetHBITMAP(Color(0, 0, 0, 0), &m_hBit);
	
	//===================================================//
	//Bitmap �ε�
	//m_hBit = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(m_hBit, sizeof(BITMAP), &m_tBitmapInfo);

	//Bitmap�� �����ų DC ����
	m_hDC = CreateCompatibleDC(CEngine::GetInst()->GetMainDC());
	HBITMAP hTempBit = (HBITMAP)SelectObject(m_hDC, m_hBit);
	DeleteObject(hTempBit);
   
	return S_OK;
}


void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	//���ҽ��� �ҷ����� �ʰ� �ؽ�ó�� ��ü������ �����ϴ� �Լ���
	//�ܻ� ���� ȭ���� ���� ���� ����� �����ذ� ���۸� ���� ���ȴ�.

	//�ܻ� ���� ȭ���� ����, ������ ��Ʈ���� ������� ������ �ػ󵵷� �����Ѵ�
	m_hBit = CreateCompatibleBitmap(CEngine::GetInst()->GetMainDC(), _iWidth, _iHeight);

	//������ ���� ��Ʈ�� �� DC�� ���� �����Ѵ�.
	m_hDC = CreateCompatibleDC(m_hDC);

	//���� ������� dc�� �������� ������ ������ ���� ��Ʈ���� �������ش�.
	//�̶�, ���� ������(���� ���� dc�� 1�ȼ��� ��Ʈ���� �������� ����Ų��.)�� ��ȯ�ȴ�.
	HBITMAP hTempBit = (HBITMAP)SelectObject(m_hDC, m_hBit);

	//��ȯ�� hTempBit�� �ʿ䰡 �����Ƿ�, �ٷ� �����Ѵ�.
	DeleteObject(hTempBit);

	//���� ��Ʈ���� ������ m_tBitmapInfo�� ������ ä���.
	GetObject(m_hBit, sizeof(BITMAP), &m_tBitmapInfo);
}

void CTexture::Resize(UINT _iWidth, UINT _iHeight)
{
	//������ �ִ� �ؽ����� ũ�⸦ �»�� �������� �������Ѵ�.

	//���ο� ��Ʈ�ʰ� DC ���� �� ���� ��Ʈ�� ����
	HBITMAP hNewBit = CreateCompatibleBitmap(CEngine::GetInst()->GetMainDC(), _iWidth, _iHeight);
	HDC hNewDC = CreateCompatibleDC(m_hDC);
	HBITMAP hTempBit = (HBITMAP)SelectObject(hNewDC, hNewBit);
	DeleteObject(hTempBit);

	//������ �ִ� �׸��� ���� ������� ������ ����
	BitBlt(hNewDC, 0, 0, m_tBitmapInfo.bmWidth, m_tBitmapInfo.bmHeight,
		m_hDC, 0, 0, SRCCOPY);

	//���������� ���� ��Ʈ��, DC ����
	DeleteObject(m_hBit);
	DeleteDC(m_hDC);

	//���ο� ��Ʈ������ ID ��ü
	m_hBit = hNewBit;
	m_hDC = hNewDC;

	//��Ʈ�� ���� ����
	GetObject(m_hBit, sizeof(BITMAP), &m_tBitmapInfo);
}
