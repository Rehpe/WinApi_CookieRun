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
	//본인의 비트맵과 DC를 삭제한다
	DeleteObject(m_hBit);
	DeleteDC(m_hDC);
}


int CTexture::Load(const wstring& _strFilePath)
{
	//GDI+ 필수 코드
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// image.png 파일을 이용하여 경로의 파일을 불러와 Image 객체를 생성합니다.
	Image* image = Image::FromFile(_strFilePath.c_str(),false);

	//불러온 image를 Clone하여 Bitmap파일로 변환 후, HBITMAP파일로 변환합니다.
	Bitmap* pBitmap = static_cast<Bitmap*>(image);//(image->Clone());
	Status status = pBitmap->GetHBITMAP(Color(0, 0, 0, 0), &m_hBit);
	
	//===================================================//
	//Bitmap 로딩
	//m_hBit = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(m_hBit, sizeof(BITMAP), &m_tBitmapInfo);

	//Bitmap과 연결시킬 DC 생성
	m_hDC = CreateCompatibleDC(CEngine::GetInst()->GetMainDC());
	HBITMAP hTempBit = (HBITMAP)SelectObject(m_hDC, m_hBit);
	DeleteObject(hTempBit);
   
	return S_OK;
}


void CTexture::Create(UINT _iWidth, UINT _iHeight)
{
	//리소스를 불러오지 않고 텍스처를 자체적으로 제작하는 함수로
	//잔상 없는 화면을 위해 렌더 결과를 복사해갈 버퍼를 위해 사용된다.

	//잔상 없는 화면을 위해, 별도의 비트맵을 윈도우와 동일한 해상도로 생성한다
	m_hBit = CreateCompatibleBitmap(CEngine::GetInst()->GetMainDC(), _iWidth, _iHeight);

	//별도로 만든 비트맵 용 DC를 따로 생성한다.
	m_hDC = CreateCompatibleDC(m_hDC);

	//새로 만들어준 dc가 렌더링할 곳으로 별도로 만든 비트맵을 지정해준다.
	//이때, 기존 목적지(새로 생긴 dc는 1픽셀의 비트맵을 목적지로 가리킨다.)가 반환된다.
	HBITMAP hTempBit = (HBITMAP)SelectObject(m_hDC, m_hBit);

	//반환된 hTempBit는 필요가 없으므로, 바로 삭제한다.
	DeleteObject(hTempBit);

	//만든 비트맵의 정보를 m_tBitmapInfo에 가져와 채운다.
	GetObject(m_hBit, sizeof(BITMAP), &m_tBitmapInfo);
}

void CTexture::Resize(UINT _iWidth, UINT _iHeight)
{
	//기존에 있던 텍스쳐의 크기를 좌상단 기준으로 재조정한다.

	//새로운 비트맵과 DC 생성 후 더미 비트맵 삭제
	HBITMAP hNewBit = CreateCompatibleBitmap(CEngine::GetInst()->GetMainDC(), _iWidth, _iHeight);
	HDC hNewDC = CreateCompatibleDC(m_hDC);
	HBITMAP hTempBit = (HBITMAP)SelectObject(hNewDC, hNewBit);
	DeleteObject(hTempBit);

	//기존에 있던 그림을 새로 만들어진 곳으로 복사
	BitBlt(hNewDC, 0, 0, m_tBitmapInfo.bmWidth, m_tBitmapInfo.bmHeight,
		m_hDC, 0, 0, SRCCOPY);

	//복사했으니 기존 비트맵, DC 삭제
	DeleteObject(m_hBit);
	DeleteDC(m_hDC);

	//새로운 비트맵으로 ID 대체
	m_hBit = hNewBit;
	m_hDC = hNewDC;

	//비트맵 정보 갱신
	GetObject(m_hBit, sizeof(BITMAP), &m_tBitmapInfo);
}
