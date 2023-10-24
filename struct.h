#pragma once
#include <wtypes.h>

//게임 제작에 필요한, 자체 제작한 구조체를 모아두는 헤더

//실수 x, y좌표를 가진 2차원 벡터 구조체
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
	//좌표 관련 오퍼레이터

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
	//벡터의 정규화
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
	DWORD_PTR	wParam;		//8바이트 정수, 이벤트에 따라 있을수도, 없을 수도 있는 인자들
	DWORD_PTR	lParam;		//8바이트 정수, 이벤트에 따라 있을수도, 없을 수도 있는 인자들
};

struct tAnimFrm
{

	Vec2	vSize;			//애니메이션 프레임 크기
	Vec2	vLeftTop;		//통짜 이미지 내의 프레임의 좌상단 좌표
	Vec2	vOffset;		//아틀라스 이미지의 미세한 픽셀 차이를 메꿔줄 오프셋
	float	fDuration;		//해당 프레임을 노출할 시간
};

struct tBtnInfo
{
	wstring		strKey;				//맵타일 이미지 이름
	wstring		strRelativePath;	//맵타일 이미지 경로
	int			iClassType;				//무슨 클래스를 만들지(obstacle, ground, platform, item, jelly, trigger 등..)
};


struct tCookieInfo
{
	wstring			strKey;					//프리뷰 이미지 이름
	wstring			strPreviewPath;			//프리뷰 이미지 경로
	SELECT_COOKIE	eCookieType;				//생성할 쿠키
};


struct tPetInfo
{
	wstring		strKey;					//프리뷰 이미지 이름
	wstring		strPreviewPath;			//프리뷰 이미지 경로
	SELECT_PET	ePetType;				//생성할 펫
};