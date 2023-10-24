#include "pch.h"
#include "CAnimation.h"
#include "CTimeManager.h"
#include "CAnimator.h"
#include "CObj.h"
#include "CCamera.h"
#include "CTexture.h"
#include "CPathManager.h"
#include "CResourceManager.h"

CAnimation::CAnimation(CAnimator* _pAnimator)
	: m_pAnimator(_pAnimator)
	, m_pAtlas(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}


//애니메이션 초기화 함수
void CAnimation::init(const wstring& _strName, CTexture* _pAtlas, Vec2 _vLeftTop, Vec2 _vSize, Vec2 _vOffset, int _iMaxFrmCount, float _fDuration)
{
	SetName(_strName);		//애니메이션의 이름 지정
	m_pAtlas = _pAtlas;		//애니메이션의 아틀라스 이미지 지정

	//애니메이션의 프레임 수만큼 루프를 돌며 프레임 구조체를 채워나간다.
	for (int i = 0; i < _iMaxFrmCount; i++)
	{
		tAnimFrm	frm = {};
		
		//i(몇번째 프레임인지)에 따라 _vSize.x(프레임의 가로 크기)만큼 곱해주어 다음 프레임의 좌상단 시작점을 잡는다. 
		frm.vLeftTop = Vec2(_vLeftTop.x + (float)i * _vSize.x, _vLeftTop.y);
		frm.fDuration = _fDuration;
		frm.vSize = _vSize;
		frm.vOffset = _vOffset;

		//채워넣은 i번째 프레임 정보를 차례차례 프레임 벡터에 삽입한다.
		m_vecFrm.push_back(frm);
	}
}

void CAnimation::tick()
{
	//애니메이션이 finish 상태라면 리턴시켜서 마지막 프레임을 계속 유지하게 한다.
	if (m_bFinish)
		return;

	//누적 시간을 기록한다.
	m_fAccTime += DT;

	//누적시간이 현재 프레임의 지속시간보다 커지면
	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime)
	{
		//다음 인덱스로
		m_iCurFrm++;

		//누적시간 초기화
		m_fAccTime = 0;

		//프레임 수가 애니메이션의 전체 프레임 수를 넘어가면 Finish 상태이다.
		if (m_iCurFrm >= m_vecFrm.size())
		{
			//애니메이션의 마지막 상태를 유지한 채로 Finish 상태값을 true로 바꿔준다.
			m_iCurFrm = int(m_vecFrm.size() - 1);
			m_bFinish = true;
		}
	}
}

void CAnimation::render(HDC _dc)
{
	//자신을 소유한 애니메이터를 타고 주인 오브젝트가 누군지 알아온다.
	CObj* pOwnerObj = m_pAnimator->GetOwner();

	//해당 오브젝트의 좌표값을 알아와 카메라 클래스를 통해 렌더링 좌표로 변환한다.
	Vec2 vPos = CCamera::GetInst()->GetRenderPos(pOwnerObj->GetPos());

	//현재 프레임 정보 구조체를 가져와 이를 바탕으로 TransparentBlt하여 렌더링한다
	tAnimFrm frm = m_vecFrm[m_iCurFrm];

	BLENDFUNCTION tBlend = {};

	float fRatio = m_pAnimator->GetRatio();
	float fSizeRatio = m_pAnimator->GetSizeRatio();
	Vec2 vOffset = m_pAnimator->GetOffset();

	tBlend.AlphaFormat = AC_SRC_ALPHA;
	tBlend.BlendFlags = 0;
	tBlend.BlendOp = AC_SRC_OVER;
	tBlend.SourceConstantAlpha = int(255.f) * fRatio;

	AlphaBlend(_dc
		, (int)((vPos.x - ((frm.vSize.x * fSizeRatio) / 2) + frm.vOffset.x + vOffset.x))
		, (int)((vPos.y - ((frm.vSize.y * fSizeRatio) / 2) + frm.vOffset.y + vOffset.y ))
		, (int)frm.vSize.x * fSizeRatio 
		, (int)frm.vSize.y * fSizeRatio
		, m_pAtlas->GetDC()
		, (int)frm.vLeftTop.x
		, (int)frm.vLeftTop.y
		, (int)frm.vSize.x
		, (int)frm.vSize.y
		, tBlend);
	//AlphaBlend(_dc
	//	, (int)((vPos.x - ((frm.vSize.x + (int)frm.vSize.x * fSizeRatio) / 2) + frm.vOffset.x))
	//	, (int)((vPos.y - ((frm.vSize.y + (int)frm.vSize.y * fSizeRatio) / 2) + frm.vOffset.y))
	//	, (int)frm.vSize.x + (int)frm.vSize.x * fSizeRatio
	//	, (int)frm.vSize.y + (int)frm.vSize.y * fSizeRatio
	//	, m_pAtlas->GetDC()
	//	, (int)frm.vLeftTop.x
	//	, (int)frm.vLeftTop.y
	//	, (int)frm.vSize.x
	//	, (int)frm.vSize.y
	//	, tBlend);
}

void CAnimation::Reset()
{
	//애니메이션이 바뀌거나 첫 세팅될 경우 프레임 및 재생시간을 초기화한다.
	m_bFinish = false;
	m_iCurFrm = 0;
	m_fAccTime = 0.f;
}

void CAnimation::Save(const wstring& _strRelativePath)
{
	//PathManager에서 기본 경로를 가져온다
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();

	//인자로 들어온 상대경로를 붙여 애니메이션을 저장할 최종 경로를 완성한다.
	strFilePath += _strRelativePath;

	//파일입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	//파일 포인터가 nullptr라면 파일 생성이 제대로 되지 않은 것으로 리턴
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 저장 실패", MB_OK);
		return;
	}

	//===== 01. 애니메이션 이름 저장=====
	//메모장에서 수정시 오류가 나지 않도록 처음에 개행문자를 저장
	fwprintf_s(pFile, L"\n");

	//가독성을 위해 맨 위에 필드명 저장
	fwprintf_s(pFile, L"[ANIMATION_NAME]\n");
	//2바이트 문자열을 파일에 저장할 때 쓰는 fwprintf_s 함수
	fwprintf_s(pFile, GetName().c_str());
	//외부에서 메모장으로 파일을 확인할 때 가독성을 위해 개행문자도 저장
	fwprintf_s(pFile, L"\n\n");

	//===== 02. 아틀라스 키(이름)값 & 경로 저장=====
	fwprintf_s(pFile, L"[ATLAS_KEY]\n");
	fwprintf_s(pFile, m_pAtlas->GetKey().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[ATLAS_PATH]\n");
	fwprintf_s(pFile, m_pAtlas->GetRelativePath().c_str());
	fwprintf_s(pFile, L"\n\n");

	//===== 03. 프레임 정보 저장=====
	fwprintf_s(pFile, L"[FRAME_COUNT]\n");
	
	wchar_t szNum[50] = {};
	//프레임 카운트는 정수형으로, 저장하려면 숫자를 문자열로 바꿔야한다.
	//_itow_s(숫자를 2바이트 문자열로 바꾸어주는 함수) 사용
	size_t iFrmCount = m_vecFrm.size();
	_itow_s((int)iFrmCount, szNum, 50, 10);

	fwprintf_s(pFile, szNum);
	fwprintf_s(pFile, L"\n\n");

	//ifrmcount만큼 반복문을 돌며 프레임 정보를 기록한다
	for (size_t i = 0; i < iFrmCount; i++)
	{
		fwprintf_s(pFile, L"[%zd_FRAME]\n", i);

		//Left Top
		fwprintf_s(pFile, L"[LEFT_TOP]\n");
		fwprintf_s(pFile, L"%.1f %.1f\n", m_vecFrm[i].vLeftTop.x, m_vecFrm[i].vLeftTop.y);

		//Size
		fwprintf_s(pFile, L"[SIZE]\n");
		fwprintf_s(pFile, L"%.1f %.1f\n", m_vecFrm[i].vSize.x, m_vecFrm[i].vSize.y);

		//Offset
		fwprintf_s(pFile, L"[OFFSET]\n");
		fwprintf_s(pFile, L"%.1f %.1f\n", m_vecFrm[i].vOffset.x, m_vecFrm[i].vOffset.y);

		//Duration
		fwprintf_s(pFile, L"[DURATION]\n");
		fwprintf_s(pFile, L"%.2f\n", m_vecFrm[i].fDuration);
		fwprintf_s(pFile, L"\n");
	}

	//파일 저장을 마치면 스트림을 닫아준다.
	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativePath)
{
	//PathManager에서 기본 경로를 가져온다
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();

	//인자로 들어온 상대경로를 붙여 애니메이션을 불러올 최종 경로를 완성한다.
	strFilePath += _strRelativePath;

	//파일입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	//파일 포인터가 nullptr라면 파일 생성이 제대로 되지 않은 것으로 리턴
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Load 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 불러오기 실패", MB_OK);
		return;
	}

	//불러올 변수들을 선언해준다
	size_t iFrameCount = 0;
	wstring strAtlasKey;
	wstring strAtlasRelativePath;

	//데이터들을 한번에 읽어올 2바이트 문자열 256칸 버퍼를 만든다.
	while (true)
	{
		wchar_t szBuffer[256] = {};
		fwscanf_s(pFile, L"%s", szBuffer, 256);

		//====읽어들인 문자열(Buffer)이 필드명과 같은지 체크하고, 같다면 애니메이션 정보로 셋팅한다===

		//애니메이션 이름
		if (!wcscmp(szBuffer, L"[ANIMATION_NAME]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			SetName(szBuffer);
		}
		//아틀라스 키값
		else if (!wcscmp(szBuffer, L"[ATLAS_KEY]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAtlasKey = szBuffer;
		}
		//아틀라스 경로값
		else if (!wcscmp(szBuffer, L"[ATLAS_PATH]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAtlasRelativePath = szBuffer;
		}
		//최대 프레임 갯수
		else if (!wcscmp(szBuffer, L"[FRAME_COUNT]"))
		{
			fwscanf_s(pFile, L"%zd", &iFrameCount);

			//프레임 갯수를 읽어온 이후에는 iframecount만큼 반복문을 돌려 이하 값들을 읽어온다
			for (size_t i = 0; i < iFrameCount; i++)
			{
				tAnimFrm frm = {};

				while (true)
				{
					//데이터들을 한번에 읽어올 2바이트 문자열 256칸 버퍼를 만든다.
					wchar_t szBuffer[256] = {};
					fwscanf_s(pFile, L"%s", szBuffer, 256);

					//좌상단값
					if (!wcscmp(szBuffer, L"[LEFT_TOP]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.vLeftTop.x, &frm.vLeftTop.y);
					}
					//프레임 크기
					else if (!wcscmp(szBuffer, L"[SIZE]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.vSize.x, &frm.vSize.y);
					}
					//오프셋
					else if (!wcscmp(szBuffer, L"[OFFSET]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.vOffset.x, &frm.vOffset.y);
					}
					//프레임 노출길이
					else if (!wcscmp(szBuffer, L"[DURATION]"))
					{
						fwscanf_s(pFile, L"%f", &frm.fDuration);
						break;
					}
				}
				//반복문을 돌며 차례대로 가져온 frm 구조체를 애니메이션 프레임 벡터에 푸시백해준다.
				m_vecFrm.push_back(frm);
			}
			break;
		}
	}
	//애니메이션의 아틀라스 이미지는 리소스 매니저에 등록해둔다.
	m_pAtlas = CResourceManager::GetInst()->LoadTexture(strAtlasKey, strAtlasRelativePath);

	//파일 로딩을 마치면 스트림을 닫아준다. 
	fclose(pFile);
}
