#include "pch.h"
#include "CBackground.h"
#include "CResourceManager.h"
#include "CTexture.h"
#include "CCamera.h"
#include "CEngine.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CPathManager.h"
#include "CLevel.h"
#include "CLevelManager.h"
#include "CStage1.h"
#include "CBonusStage.h"


CBackground::CBackground()
	: m_vBG1Pos(0.f,0.f)
	, m_vBG2Pos(0.f,0.f)
	, m_vBG3Pos(0.f, 0.f)
	, m_vBG4Pos(0.f, 0.f)
{
	m_pBG1Tex = CResourceManager::GetInst()->LoadTexture(L"background1", L"texture\\map\\Background\\background.png");
	m_pBG2Tex = CResourceManager::GetInst()->LoadTexture(L"bgCastle", L"texture\\map\\Background\\bgCastle.png");
	m_pBG3Tex = CResourceManager::GetInst()->LoadTexture(L"bgCloud", L"texture\\map\\Background\\bgCloud.png");
	m_pBG4Tex = nullptr;

}

CBackground::~CBackground()
{
}

void CBackground::init()
{
	CLevel* pLevel = CLevelManager::GetInst()->GetCurLevel();

	//Stage1�̶��
	if (pLevel == dynamic_cast<CStage1*>(pLevel))
	{
		m_pBG1Tex = CResourceManager::GetInst()->LoadTexture(L"background1", L"texture\\map\\Background\\background.png");
		m_pBG2Tex = CResourceManager::GetInst()->LoadTexture(L"bgCastle", L"texture\\map\\Background\\bgCastle.png");
		m_pBG3Tex = CResourceManager::GetInst()->LoadTexture(L"bgCloud", L"texture\\map\\Background\\bgCloud.png");
		m_pBG4Tex = nullptr;
	}

	//Bonus Stage���
	if (pLevel == dynamic_cast<CBonusStage*>(pLevel))
	{
		m_pBG1Tex = CResourceManager::GetInst()->LoadTexture(L"bonusBG1", L"texture\\map\\Background\\bgBonus1.png");
		m_pBG2Tex = CResourceManager::GetInst()->LoadTexture(L"bonusBG2", L"texture\\map\\Background\\bgBonus2.png");
		m_pBG3Tex = CResourceManager::GetInst()->LoadTexture(L"bonusBG3", L"texture\\map\\Background\\bgBonus3.png");
		m_pBG4Tex = CResourceManager::GetInst()->LoadTexture(L"bonusBG4", L"texture\\map\\Background\\bgBonus4.png");
	}
}

void CBackground::tick()
{
	m_vBG1Pos += Vec2(8.f, 0.f) * DT;
	m_vBG2Pos += Vec2(15.f, 0.f) * DT;
	m_vBG3Pos += Vec2(20.f, 0.f) * DT;
	m_vBG4Pos += Vec2(30.f, 0.f) * DT;
	
}
	

void CBackground::render(HDC _dc)
{
	BG1Loop(_dc);
	BG2Loop(_dc);
	BG3Loop(_dc);
	BG4Loop(_dc);

	CObj::render(_dc);
}

void CBackground::BG1Loop(HDC _dc)
{
	if (m_pBG1Tex == nullptr)
		return;

	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	if (m_vBG1Pos.x < 0)
	{
		BitBlt(_dc, 0, 0, -m_vBG1Pos.x, m_pBG1Tex->GetHeight(),
			m_pBG1Tex->GetDC(), m_pBG1Tex->GetWidth() + m_vBG1Pos.x, 0, SRCCOPY);

		BitBlt(_dc, -m_vBG1Pos.x, 0, vResolution.x + m_vBG1Pos.x, m_pBG1Tex->GetHeight(),
			m_pBG1Tex->GetDC(), 0, 0, SRCCOPY);
	}
	else
	{
		BitBlt(_dc, 0, 0, vResolution.x, m_pBG1Tex->GetHeight(),
			m_pBG1Tex->GetDC(), m_vBG1Pos.x, 0, SRCCOPY);
	}

	if (m_vBG1Pos.x + vResolution.x >= m_pBG1Tex->GetWidth())
	{
		m_vBG1Pos.x -= m_pBG1Tex->GetWidth();
	}
}

void CBackground::BG2Loop(HDC _dc)
{
	if (m_pBG2Tex == nullptr)
		return;

	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	//AlphaBlend ����
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;				// �Ϲ� ��Ʈ�� 0, 32��Ʈ ��Ʈ�� AC_SRC_ALPHA
	tBlend.BlendFlags = 0;							// 0
	tBlend.BlendOp = AC_SRC_OVER;					// AC_SRC_OVER
	tBlend.SourceConstantAlpha = int(255.f);		// ���� (0) ~ ������ (255)

	if (m_vBG2Pos.x < 0)
	{
		AlphaBlend(_dc										// ����� DC
			, 0.f, 0.f							// ��� ������ �»�� ��ǥ	
			, -m_vBG2Pos.x, vResolution.y				// ����� ����, ���� ũ��
			, m_pBG2Tex->GetDC()								// ������ DC
			, m_pBG2Tex->GetWidth() + m_vBG2Pos.x, 0						// ������ �̹����� ���� ��ġ(��� ��ǥ�������� �ܾ������)
			, -m_vBG2Pos.x, vResolution.y					// ������ �̹����� ����, ���� ũ��
			, tBlend);

		AlphaBlend(_dc										// ����� DC
			, -m_vBG2Pos.x, 0.f							// ��� ������ �»�� ��ǥ	
			, vResolution.x + m_vBG2Pos.x, vResolution.y				// ����� ����, ���� ũ��
			, m_pBG2Tex->GetDC()								// ������ DC
			, 0, 0						// ������ �̹����� ���� ��ġ(��� ��ǥ�������� �ܾ������)
			, vResolution.x + m_vBG2Pos.x, vResolution.y					// ������ �̹����� ����, ���� ũ��
			, tBlend);
	}
	else
	{
		AlphaBlend(_dc										// ����� DC
			, 0.f,0.f							// ��� ������ �»�� ��ǥ	
			, vResolution.x, vResolution.y				// ����� ����, ���� ũ��
			, m_pBG2Tex->GetDC()								// ������ DC
			, m_vBG2Pos.x, 0						// ������ �̹����� ���� ��ġ(��� ��ǥ�������� �ܾ������)
			, vResolution.x, vResolution.y					// ������ �̹����� ����, ���� ũ��
			, tBlend);
	}

	if (m_vBG2Pos.x + vResolution.x >= m_pBG2Tex->GetWidth())
	{
		m_vBG2Pos.x -= m_pBG2Tex->GetWidth();
	}

	////AlphaBlend ����
	//BLENDFUNCTION tBlend = {};

	//tBlend.AlphaFormat = AC_SRC_ALPHA;				// �Ϲ� ��Ʈ�� 0, 32��Ʈ ��Ʈ�� AC_SRC_ALPHA
	//tBlend.BlendFlags = 0;							// 0
	//tBlend.BlendOp = AC_SRC_OVER;					// AC_SRC_OVER
	//tBlend.SourceConstantAlpha = int(255.f);		// ���� (0) ~ ������ (255)

	//AlphaBlend(_dc										// ����� DC
	//	, 0.f, 0.f							// ��� ������ �»�� ��ǥ	
	//	, vResolution.x, vResolution.y				// ����� ����, ���� ũ��
	//	, m_pBG2Tex->GetDC()								// ������ DC
	//	, m_vBG2Pos.x, 0						// ������ �̹����� ���� ��ġ(��� ��ǥ�������� �ܾ������)
	//	, vResolution.x, vResolution.y					// ������ �̹����� ����, ���� ũ��
	//	, tBlend);

	//float fEndAtlas_x = m_vBG2Pos.x + vResolution.x;
	//if (fEndAtlas_x >= m_pBG2Tex->GetWidth())
	//{
	//	float fDiff = fEndAtlas_x - m_pBG2Tex->GetWidth();
	//	m_vBG2Pos.x = fDiff;
	//}
}

void CBackground::BG3Loop(HDC _dc)
{
	if (m_pBG3Tex == nullptr)
		return;

	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	//AlphaBlend ����
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;				// �Ϲ� ��Ʈ�� 0, 32��Ʈ ��Ʈ�� AC_SRC_ALPHA
	tBlend.BlendFlags = 0;							// 0
	tBlend.BlendOp = AC_SRC_OVER;					// AC_SRC_OVER
	tBlend.SourceConstantAlpha = int(255.f);		// ���� (0) ~ ������ (255)

	if (m_vBG3Pos.x < 0)
	{
		AlphaBlend(_dc										// ����� DC
			, 0.f, 0.f							// ��� ������ �»�� ��ǥ	
			, -m_vBG3Pos.x, vResolution.y				// ����� ����, ���� ũ��
			, m_pBG3Tex->GetDC()								// ������ DC
			, m_pBG3Tex->GetWidth() + m_vBG3Pos.x, 0						// ������ �̹����� ���� ��ġ(��� ��ǥ�������� �ܾ������)
			, -m_vBG3Pos.x, vResolution.y					// ������ �̹����� ����, ���� ũ��
			, tBlend);

		AlphaBlend(_dc										// ����� DC
			, -m_vBG3Pos.x, 0.f							// ��� ������ �»�� ��ǥ	
			, vResolution.x + m_vBG3Pos.x, vResolution.y				// ����� ����, ���� ũ��
			, m_pBG3Tex->GetDC()								// ������ DC
			, 0, 0						// ������ �̹����� ���� ��ġ(��� ��ǥ�������� �ܾ������)
			, vResolution.x + m_vBG3Pos.x, vResolution.y					// ������ �̹����� ����, ���� ũ��
			, tBlend);
	}
	else
	{
		AlphaBlend(_dc										// ����� DC
			, 0.f, 0.f							// ��� ������ �»�� ��ǥ	
			, vResolution.x, vResolution.y				// ����� ����, ���� ũ��
			, m_pBG3Tex->GetDC()								// ������ DC
			, m_vBG3Pos.x, 0						// ������ �̹����� ���� ��ġ(��� ��ǥ�������� �ܾ������)
			, vResolution.x, vResolution.y					// ������ �̹����� ����, ���� ũ��
			, tBlend);
	}

	if (m_vBG3Pos.x + vResolution.x >= m_pBG3Tex->GetWidth())
	{
		m_vBG3Pos.x -= m_pBG3Tex->GetWidth();
	}
}

void CBackground::BG4Loop(HDC _dc)
{
	if (m_pBG4Tex == nullptr)
		return;

	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	//AlphaBlend ����
	BLENDFUNCTION tBlend = {};

	tBlend.AlphaFormat = AC_SRC_ALPHA;				// �Ϲ� ��Ʈ�� 0, 32��Ʈ ��Ʈ�� AC_SRC_ALPHA
	tBlend.BlendFlags = 0;							// 0
	tBlend.BlendOp = AC_SRC_OVER;					// AC_SRC_OVER
	tBlend.SourceConstantAlpha = int(255.f);		// ���� (0) ~ ������ (255)

	if (m_vBG4Pos.x < 0)
	{
		AlphaBlend(_dc										// ����� DC
			, 0.f, 0.f							// ��� ������ �»�� ��ǥ	
			, -m_vBG4Pos.x, vResolution.y				// ����� ����, ���� ũ��
			, m_pBG4Tex->GetDC()								// ������ DC
			, m_pBG4Tex->GetWidth() + m_vBG4Pos.x, 0						// ������ �̹����� ���� ��ġ(��� ��ǥ�������� �ܾ������)
			, -m_vBG4Pos.x, vResolution.y					// ������ �̹����� ����, ���� ũ��
			, tBlend);

		AlphaBlend(_dc										// ����� DC
			, -m_vBG4Pos.x, 0.f							// ��� ������ �»�� ��ǥ	
			, vResolution.x + m_vBG4Pos.x, vResolution.y				// ����� ����, ���� ũ��
			, m_pBG4Tex->GetDC()								// ������ DC
			, 0, 0						// ������ �̹����� ���� ��ġ(��� ��ǥ�������� �ܾ������)
			, vResolution.x + m_vBG4Pos.x, vResolution.y					// ������ �̹����� ����, ���� ũ��
			, tBlend);
	}
	else
	{
		AlphaBlend(_dc										// ����� DC
			, 0.f, 0.f							// ��� ������ �»�� ��ǥ	
			, vResolution.x, vResolution.y				// ����� ����, ���� ũ��
			, m_pBG4Tex->GetDC()								// ������ DC
			, m_vBG4Pos.x, 0						// ������ �̹����� ���� ��ġ(��� ��ǥ�������� �ܾ������)
			, vResolution.x, vResolution.y					// ������ �̹����� ����, ���� ũ��
			, tBlend);
	}

	if (m_vBG4Pos.x + vResolution.x >= m_pBG4Tex->GetWidth())
	{
		m_vBG4Pos.x -= m_pBG4Tex->GetWidth();
	}
}


