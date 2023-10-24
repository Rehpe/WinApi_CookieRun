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


//�ִϸ��̼� �ʱ�ȭ �Լ�
void CAnimation::init(const wstring& _strName, CTexture* _pAtlas, Vec2 _vLeftTop, Vec2 _vSize, Vec2 _vOffset, int _iMaxFrmCount, float _fDuration)
{
	SetName(_strName);		//�ִϸ��̼��� �̸� ����
	m_pAtlas = _pAtlas;		//�ִϸ��̼��� ��Ʋ�� �̹��� ����

	//�ִϸ��̼��� ������ ����ŭ ������ ���� ������ ����ü�� ä��������.
	for (int i = 0; i < _iMaxFrmCount; i++)
	{
		tAnimFrm	frm = {};
		
		//i(���° ����������)�� ���� _vSize.x(�������� ���� ũ��)��ŭ �����־� ���� �������� �»�� �������� ��´�. 
		frm.vLeftTop = Vec2(_vLeftTop.x + (float)i * _vSize.x, _vLeftTop.y);
		frm.fDuration = _fDuration;
		frm.vSize = _vSize;
		frm.vOffset = _vOffset;

		//ä������ i��° ������ ������ �������� ������ ���Ϳ� �����Ѵ�.
		m_vecFrm.push_back(frm);
	}
}

void CAnimation::tick()
{
	//�ִϸ��̼��� finish ���¶�� ���Ͻ��Ѽ� ������ �������� ��� �����ϰ� �Ѵ�.
	if (m_bFinish)
		return;

	//���� �ð��� ����Ѵ�.
	m_fAccTime += DT;

	//�����ð��� ���� �������� ���ӽð����� Ŀ����
	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime)
	{
		//���� �ε�����
		m_iCurFrm++;

		//�����ð� �ʱ�ȭ
		m_fAccTime = 0;

		//������ ���� �ִϸ��̼��� ��ü ������ ���� �Ѿ�� Finish �����̴�.
		if (m_iCurFrm >= m_vecFrm.size())
		{
			//�ִϸ��̼��� ������ ���¸� ������ ä�� Finish ���°��� true�� �ٲ��ش�.
			m_iCurFrm = int(m_vecFrm.size() - 1);
			m_bFinish = true;
		}
	}
}

void CAnimation::render(HDC _dc)
{
	//�ڽ��� ������ �ִϸ����͸� Ÿ�� ���� ������Ʈ�� ������ �˾ƿ´�.
	CObj* pOwnerObj = m_pAnimator->GetOwner();

	//�ش� ������Ʈ�� ��ǥ���� �˾ƿ� ī�޶� Ŭ������ ���� ������ ��ǥ�� ��ȯ�Ѵ�.
	Vec2 vPos = CCamera::GetInst()->GetRenderPos(pOwnerObj->GetPos());

	//���� ������ ���� ����ü�� ������ �̸� �������� TransparentBlt�Ͽ� �������Ѵ�
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
	//�ִϸ��̼��� �ٲ�ų� ù ���õ� ��� ������ �� ����ð��� �ʱ�ȭ�Ѵ�.
	m_bFinish = false;
	m_iCurFrm = 0;
	m_fAccTime = 0.f;
}

void CAnimation::Save(const wstring& _strRelativePath)
{
	//PathManager���� �⺻ ��θ� �����´�
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();

	//���ڷ� ���� ����θ� �ٿ� �ִϸ��̼��� ������ ���� ��θ� �ϼ��Ѵ�.
	strFilePath += _strRelativePath;

	//���������
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	//���� �����Ͱ� nullptr��� ���� ������ ����� ���� ���� ������ ����
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Save ����, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"���� ���� ����", MB_OK);
		return;
	}

	//===== 01. �ִϸ��̼� �̸� ����=====
	//�޸��忡�� ������ ������ ���� �ʵ��� ó���� ���๮�ڸ� ����
	fwprintf_s(pFile, L"\n");

	//�������� ���� �� ���� �ʵ�� ����
	fwprintf_s(pFile, L"[ANIMATION_NAME]\n");
	//2����Ʈ ���ڿ��� ���Ͽ� ������ �� ���� fwprintf_s �Լ�
	fwprintf_s(pFile, GetName().c_str());
	//�ܺο��� �޸������� ������ Ȯ���� �� �������� ���� ���๮�ڵ� ����
	fwprintf_s(pFile, L"\n\n");

	//===== 02. ��Ʋ�� Ű(�̸�)�� & ��� ����=====
	fwprintf_s(pFile, L"[ATLAS_KEY]\n");
	fwprintf_s(pFile, m_pAtlas->GetKey().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[ATLAS_PATH]\n");
	fwprintf_s(pFile, m_pAtlas->GetRelativePath().c_str());
	fwprintf_s(pFile, L"\n\n");

	//===== 03. ������ ���� ����=====
	fwprintf_s(pFile, L"[FRAME_COUNT]\n");
	
	wchar_t szNum[50] = {};
	//������ ī��Ʈ�� ����������, �����Ϸ��� ���ڸ� ���ڿ��� �ٲ���Ѵ�.
	//_itow_s(���ڸ� 2����Ʈ ���ڿ��� �ٲپ��ִ� �Լ�) ���
	size_t iFrmCount = m_vecFrm.size();
	_itow_s((int)iFrmCount, szNum, 50, 10);

	fwprintf_s(pFile, szNum);
	fwprintf_s(pFile, L"\n\n");

	//ifrmcount��ŭ �ݺ����� ���� ������ ������ ����Ѵ�
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

	//���� ������ ��ġ�� ��Ʈ���� �ݾ��ش�.
	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativePath)
{
	//PathManager���� �⺻ ��θ� �����´�
	wstring strFilePath = CPathManager::GetInst()->GetContentPath();

	//���ڷ� ���� ����θ� �ٿ� �ִϸ��̼��� �ҷ��� ���� ��θ� �ϼ��Ѵ�.
	strFilePath += _strRelativePath;

	//���������
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	//���� �����Ͱ� nullptr��� ���� ������ ����� ���� ���� ������ ����
	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Load ����, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"���� �ҷ����� ����", MB_OK);
		return;
	}

	//�ҷ��� �������� �������ش�
	size_t iFrameCount = 0;
	wstring strAtlasKey;
	wstring strAtlasRelativePath;

	//�����͵��� �ѹ��� �о�� 2����Ʈ ���ڿ� 256ĭ ���۸� �����.
	while (true)
	{
		wchar_t szBuffer[256] = {};
		fwscanf_s(pFile, L"%s", szBuffer, 256);

		//====�о���� ���ڿ�(Buffer)�� �ʵ��� ������ üũ�ϰ�, ���ٸ� �ִϸ��̼� ������ �����Ѵ�===

		//�ִϸ��̼� �̸�
		if (!wcscmp(szBuffer, L"[ANIMATION_NAME]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			SetName(szBuffer);
		}
		//��Ʋ�� Ű��
		else if (!wcscmp(szBuffer, L"[ATLAS_KEY]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAtlasKey = szBuffer;
		}
		//��Ʋ�� ��ΰ�
		else if (!wcscmp(szBuffer, L"[ATLAS_PATH]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAtlasRelativePath = szBuffer;
		}
		//�ִ� ������ ����
		else if (!wcscmp(szBuffer, L"[FRAME_COUNT]"))
		{
			fwscanf_s(pFile, L"%zd", &iFrameCount);

			//������ ������ �о�� ���Ŀ��� iframecount��ŭ �ݺ����� ���� ���� ������ �о�´�
			for (size_t i = 0; i < iFrameCount; i++)
			{
				tAnimFrm frm = {};

				while (true)
				{
					//�����͵��� �ѹ��� �о�� 2����Ʈ ���ڿ� 256ĭ ���۸� �����.
					wchar_t szBuffer[256] = {};
					fwscanf_s(pFile, L"%s", szBuffer, 256);

					//�»�ܰ�
					if (!wcscmp(szBuffer, L"[LEFT_TOP]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.vLeftTop.x, &frm.vLeftTop.y);
					}
					//������ ũ��
					else if (!wcscmp(szBuffer, L"[SIZE]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.vSize.x, &frm.vSize.y);
					}
					//������
					else if (!wcscmp(szBuffer, L"[OFFSET]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.vOffset.x, &frm.vOffset.y);
					}
					//������ �������
					else if (!wcscmp(szBuffer, L"[DURATION]"))
					{
						fwscanf_s(pFile, L"%f", &frm.fDuration);
						break;
					}
				}
				//�ݺ����� ���� ���ʴ�� ������ frm ����ü�� �ִϸ��̼� ������ ���Ϳ� Ǫ�ù����ش�.
				m_vecFrm.push_back(frm);
			}
			break;
		}
	}
	//�ִϸ��̼��� ��Ʋ�� �̹����� ���ҽ� �Ŵ����� ����صд�.
	m_pAtlas = CResourceManager::GetInst()->LoadTexture(strAtlasKey, strAtlasRelativePath);

	//���� �ε��� ��ġ�� ��Ʈ���� �ݾ��ش�. 
	fclose(pFile);
}
