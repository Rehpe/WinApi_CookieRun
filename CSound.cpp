#include "pch.h"
#include "CSound.h"

CSound::CSound()
	: m_pSoundBuffer(nullptr)
	, m_tBuffInfo{}
	, m_iVolume(0)
{
}

CSound::~CSound()
{
	if (m_pSoundBuffer != nullptr)
		m_pSoundBuffer->Release();
}

int CSound::Load(const wstring& _strFilePath)
{
	if (CSoundManager::GetInst()->GetSoundDevice() == nullptr)
		assert(nullptr); // ���� ��ü �������� ����

	// Ȯ���� �̸� �����ϱ�
	wchar_t szExt[10] = { 0 };
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 10);

	if (!wcscmp(szExt, L".wav")) // WAV ���� �ε�
	{
		if (LoadWaveSound(_strFilePath) == false)
			assert(nullptr);
	}
	else
		assert(nullptr);

	return S_OK;
}

void CSound::Play(bool _bLoop)
{
	// Play �Լ��� 1��° 2��° ���ڴ� 0 ���� �̹� ����Ǿ��ִ�.
	// 3��° ������ ���带 �ݺ���� �� ������ �ƴ����� �����Ѵ�.
	// m_pSoundBuffer->SetCurrentPosition(0);

	if (_bLoop)
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	else
		m_pSoundBuffer->Play(0, 0, 0);
}

void CSound::PlayToBGM(bool _bLoop)
{
	//�ش� ���带 BGM���� ����Ѵ�.
	CSoundManager::GetInst()->RegisterToBGM(this);

	// Play �Լ��� 1��° 2��° ���ڴ� 0 ���� �̹� ����Ǿ��ִ�.
	// 3��° ������ ���带 �ݺ���� �� ������ �ƴ����� �����Ѵ�.
	if (_bLoop)
		m_pSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	else
		m_pSoundBuffer->Play(0, 0, 0);
}

void CSound::Stop(bool _bReset)
{
	m_pSoundBuffer->Stop();

	//Reset���� true��� ������ ���������� ó������ �������´�
	if (_bReset)
		m_pSoundBuffer->SetCurrentPosition(0);
}

void CSound::SetVolume(float _fVolume)
{
	m_iVolume = GetDecibel(_fVolume);
	m_pSoundBuffer->SetVolume(m_iVolume);
}

void CSound::SetPosition(float _fPosition)
{
	//�ϴ� ������ ���������� 0�ʷ� �������´�. 
	Stop(true);

	//������ ��ü ���̸� 100������ ������ �ش� �κ����� ���������� �����Ѵ�
	DWORD dwBytes = (DWORD)((_fPosition / 100.f) * (float)m_tBuffInfo.dwBufferBytes);
	m_pSoundBuffer->SetCurrentPosition(dwBytes);
}

bool CSound::LoadWaveSound(const wstring& _strFilePath)
{
	HMMIO	hFile; // File Handle

	wstring strFilePath = _strFilePath;

	//CreateFile
	hFile = mmioOpen((wchar_t*)strFilePath.c_str(), NULL, MMIO_READ); //wave������ ����.

	if (hFile == nullptr)
	{
		MessageBox(NULL, L"���� ���ҽ� ��ο� ���� ����", L"���� �ε� ����", MB_OK);
		return false;
	}

	//Chunk ûũ ����ü, ���ڿ��� ������ �ν��ؼ� WaveFormat �� ���ۼ��������� �о�´�.
	MMCKINFO	pParent;
	memset(&pParent, 0, sizeof(pParent));
	pParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmioDescend(hFile, &pParent, NULL, MMIO_FINDRIFF);

	MMCKINFO	pChild;
	memset(&pChild, 0, sizeof(pChild));
	pChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	WAVEFORMATEX	wft;
	memset(&wft, 0, sizeof(wft));
	mmioRead(hFile, (char*)&wft, sizeof(wft));

	mmioAscend(hFile, &pChild, 0);
	pChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	memset(&m_tBuffInfo, 0, sizeof(DSBUFFERDESC));
	m_tBuffInfo.dwBufferBytes = pChild.cksize;
	m_tBuffInfo.dwSize = sizeof(DSBUFFERDESC);
	m_tBuffInfo.dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLVOLUME;
	m_tBuffInfo.lpwfxFormat = &wft;

	if (FAILED(CSoundManager::GetInst()->GetSoundDevice()->CreateSoundBuffer(&m_tBuffInfo, &m_pSoundBuffer, NULL)))
	{
		MessageBox(NULL, L"������ۻ�������", L"", MB_OK);
		return false;
	}

	void* pWrite1 = NULL;
	void* pWrite2 = NULL;
	DWORD dwlength1, dwlength2;

	m_pSoundBuffer->Lock(0, pChild.cksize, &pWrite1, &dwlength1
		, &pWrite2, &dwlength2, 0L);

	if (pWrite1 != nullptr)
		mmioRead(hFile, (char*)pWrite1, dwlength1);
	if (pWrite2 != nullptr)
		mmioRead(hFile, (char*)pWrite2, dwlength2);

	m_pSoundBuffer->Unlock(pWrite1, dwlength1, pWrite2, dwlength2);

	mmioClose(hFile, 0);

	// �ʱ� ���� �������� ����
	SetVolume(50.f);

	return true;
}

int CSound::GetDecibel(float _fVolume)
{
	//100������ ������ �� �ִ� ������ ��ȯ�ϴ� �Լ�
	
	if (_fVolume > 100.f)
		_fVolume = 100.f;
	else if (_fVolume <= 0.f)
		_fVolume = 0.00001f;

	// 1 ~ 100 ���̰��� ���ú� ������ ����
	int iVolume = (LONG)(-2000.0 * log10(100.f / _fVolume));

	if (iVolume < -10000)
		iVolume = -10000;
	return  iVolume;
}
