#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����
#include <windows.h>
// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <commdlg.h>
#include <crtdbg.h>
#include <assert.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <stack>

#include <objidl.h>
#include <gdiplus.h>

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::string;
using std::wstring;
using namespace Gdiplus;

#include "def.h"
#include "struct.h"
#include "func.h"


#pragma comment(lib, "Msimg32.lib")


#pragma comment(lib, "Gdiplus.lib")
