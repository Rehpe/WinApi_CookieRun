// main.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "framework.h"
#include "CookieRun_OvenBreak.h"
#include "CEngine.h"
#include <vector>
using std::vector;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE      hInst;                //현재 인스턴스입니다.
HWND           g_hWnd;               //메인 윈도우 핸들               

const wchar_t* g_pTitle = L"CookieRun OvenBreak"; //윈도우 창 제목

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM); //메세지 처리 관련 함수
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,  //불필요 인자
	_In_ LPWSTR    lpCmdLine,          //불필요 인자
	_In_ int       nCmdShow)           //윈도우 창을 보여줄 것인지 말지
{
	// 00. 메모리 누수 탐지
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//메모리 누수가 발생한 경우, 해당 번호를 괄호 안에 입력하면 누수가 일어난 함수의 호출 스택을 보여준다.
	//_CrtSetBreakAlloc(747891);

	//01. 윈도우 설정 셋팅
	MyRegisterClass(hInstance);

	//02. 윈도우 생성 및 초기화
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	//03. 게임 엔진 초기화
	CEngine::GetInst()->init(g_hWnd, 1280, 720);

	//04. 단축키 테이블 정보 가져옴
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COOKIERUNOVENBREAK));

	//05. 메세지 루프(PeekMessage 사용)
	MSG msg = {};

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			//가져온 메세지가 윈도우 종료 메세지라면 break;
			if (msg.message == WM_QUIT)
				break;

			//가져온 메세지가 단축키 조합에 있는지 확인
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg); //분석
				DispatchMessage(&msg);  //처리
			}
		}

		//메세지가 없으면 게임 엔진 동작
		else
		{
			//Run Game
			CEngine::GetInst()->progress();
		}
	}
		return (int)msg.wParam;
}

	//Window 창 Setting 함수
	ATOM MyRegisterClass(HINSTANCE hInstance)
	{
		WNDCLASSEXW wcex = {};

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.lpszClassName	= L"MySetting";
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COOKIERUNOVENBREAK));
		wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName	= nullptr; //MAKEINTRESOURCEW(IDC_COOKIERUNOVENBREAK);
		wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		return RegisterClassExW(&wcex);
	}

	//Window 창 생성 함수
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
	{
		hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

		g_hWnd = CreateWindowW(L"MySetting", g_pTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

		if (!g_hWnd)
		{
			return FALSE;
		}

		ShowWindow(g_hWnd, nCmdShow);
		UpdateWindow(g_hWnd);

		return TRUE;
	}

	//Window의 메세지 처리 함수
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 메뉴 선택을 구문 분석합니다:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	// 정보 대화 상자의 메시지 처리기입니다.
	INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (message)
		{
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	}
