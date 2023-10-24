#pragma once
#include <wtypes.h>

//���� ���ۿ� �ʿ���, ��ü ������ ����ü�� ��Ƶδ� ���

//�Ǽ� x, y��ǥ�� ���� 2���� ���� ����ü
struct Vec2
{
	float x;
	float y;

public:
	Vec2()
		:x(0.f)
		, y(0.f)
	{}

	Vec2(float _x, float _y)
		: x(_x), y(_y)
	{}

	Vec2(UINT _x, UINT _y)
		: x((float)_x), y((float)_y)
	{}

	Vec2(POINT _pt)
		: x((float)_pt.x)
		, y((float)_pt.y)
	{

	}

	~Vec2()
	{}


public:
	//��ǥ ���� ���۷�����

	Vec2 operator +(Vec2 _vOther)
	{
		return Vec2(x + _vOther.x, y + _vOther.y);
	}

	void operator +=(Vec2 _vOther)
	{
		x += _vOther.x;
		y += _vOther.y;
	}

	Vec2 operator -(Vec2 _vOther)
	{
		return Vec2(x - _vOther.x, y - _vOther.y);
	}

	void operator -=(Vec2 _vOther)
	{
		x -= _vOther.x;
		y -= _vOther.y;
	}

	Vec2 operator / (float _f)
	{
		return Vec2(x / _f, y / _f);
	}

	void operator /= (float _f)
	{
		x /= _f;
		y /= _f;
	}

	Vec2 operator /(Vec2 _vOther)
	{
		return Vec2(x / _vOther.x, y / _vOther.y);
	}

	void operator /=(Vec2 _vOther)
	{
		x /= _vOther.x;
		y /= _vOther.y;
	}

	Vec2 operator * (float _f)
	{
		return Vec2(x * _f, y * _f);
	}

	void operator *= (float _f)
	{
		x *= _f;
		y *= _f;
	}

	Vec2 operator *(Vec2 _vOther)
	{
		return Vec2(x * _vOther.x, y * _vOther.y);
	}

	void operator *=(Vec2 _vOther)
	{
		x *= _vOther.x;
		y *= _vOther.y;
	}

	Vec2 operator -()
	{
		return Vec2(-x, -y);
	}


public:
	//������ ����ȭ
	void Normalize()
	{
		float fDist = sqrtf(x * x + y * y);
		x /= fDist;
		y /= fDist;
	}

	float Length()
	{
		return sqrtf(x * x + y * y);
	}

	bool IsZero()
	{
		return x == 0.f && y == 0.f;
	}

};

struct tEvent
{
	EVENT_TYPE	eType;
	DWORD_PTR	wParam;		//8����Ʈ ����, �̺�Ʈ�� ���� ��������, ���� ���� �ִ� ���ڵ�
	DWORD_PTR	lParam;		//8����Ʈ ����, �̺�Ʈ�� ���� ��������, ���� ���� �ִ� ���ڵ�
};

struct tAnimFrm
{

	Vec2	vSize;			//�ִϸ��̼� ������ ũ��
	Vec2	vLeftTop;		//��¥ �̹��� ���� �������� �»�� ��ǥ
	Vec2	vOffset;		//��Ʋ�� �̹����� �̼��� �ȼ� ���̸� �޲��� ������
	float	fDuration;		//�ش� �������� ������ �ð�
};

struct tBtnInfo
{
	wstring		strKey;				//��Ÿ�� �̹��� �̸�
	wstring		strRelativePath;	//��Ÿ�� �̹��� ���
	int			iClassType;				//���� Ŭ������ ������(obstacle, ground, platform, item, jelly, trigger ��..)
};


struct tCookieInfo
{
	wstring			strKey;					//������ �̹��� �̸�
	wstring			strPreviewPath;			//������ �̹��� ���
	SELECT_COOKIE	eCookieType;				//������ ��Ű
};


struct tPetInfo
{
	wstring		strKey;					//������ �̹��� �̸�
	wstring		strPreviewPath;			//������ �̹��� ���
	SELECT_PET	ePetType;				//������ ��
};