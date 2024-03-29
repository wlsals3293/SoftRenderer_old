// SoftRenderer.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"


int g_nClientWidth = 640;
int g_nClientHeight = 480;
bool g_bIsActive;
Texture* g_Texture = NULL;

GameObject2D gameObject1;
GameObject2D gameObject2;
GameObject2D gameObject3;
GameObject2D gameObject4;
GameObject2D gameObject5;


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
WCHAR szDeveloper[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int MsgLoop(HACCEL hAccelTable);
BOOL WindowInit(HINSTANCE hInstance, int nCmdShow);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
	g_Texture = new Texture();

	Vertex v1(Vector3::Make2DPoint(-40, -40), RGB32(255, 0, 0), Vector2(0.125f, 0.25f));
	Vertex v2(Vector3::Make2DPoint(-40, 40), RGB32(0, 255, 0), Vector2(0.125f, 0.125f));
	Vertex v3(Vector3::Make2DPoint(40, 40), RGB32(0, 0, 255), Vector2(0.25f, 0.125f));
	Vertex v4(Vector3::Make2DPoint(40, -40), RGB32(255, 255, 255), Vector2(0.25f, 0.25f));

	Triangle* triangles = new Triangle[2];
	triangles[0] = Triangle(v1, v2, v3);
	triangles[1] = Triangle(v1, v3, v4);

	gameObject1.mesh.Triangles = triangles;
	gameObject1.mesh.TriangleCount = 2;

	gameObject2.mesh.Triangles = triangles;
	gameObject2.mesh.TriangleCount = 2;

	gameObject3.mesh.Triangles = triangles;
	gameObject3.mesh.TriangleCount = 2;

	gameObject4.mesh.Triangles = triangles;
	gameObject4.mesh.TriangleCount = 2;

	gameObject5.mesh.Triangles = triangles;
	gameObject5.mesh.TriangleCount = 2;


	gameObject1.SetPosition(30, 60);
	gameObject2.SetPosition(300, -40);
	gameObject3.SetPosition(200, 56);
	gameObject4.SetPosition(-300, 0);
	gameObject5.SetPosition(-180, 160);


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SOFTRENDERER, szWindowClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_DEVELOPER, szDeveloper, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!WindowInit(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOFTRENDERER));
	return MsgLoop(hAccelTable);
}


//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOFTRENDERER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SOFTRENDERER);
	wcex.lpszClassName = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
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
	case WM_CREATE:
		{
			InitGDI(hWnd);
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		ReleaseGDI(hWnd);
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

	
	g_Texture->LoadBMP("test.bmp");

    return (INT_PTR)FALSE;
}

int MsgLoop(HACCEL hAccelTable)
{
	MSG msg;

	// 기본 메시지 루프입니다.
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, nullptr, 0, 0))
			{
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

			}
			else
			{
				return (int)msg.wParam;
			}
		}
		else if (g_bIsActive)
		{
			UpdateFrame();
		}
		else
		{
			WaitMessage();
		}

	}

	return (int)msg.wParam;
}

BOOL WindowInit(HINSTANCE hInstance, int nCmdShow)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = g_nClientWidth - 1;
	rect.bottom = g_nClientHeight - 1;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
	int WindowWidth = rect.right - rect.left + 1;
	int WindowHeight = rect.bottom - rect.top + 1;

	HWND hWnd = CreateWindowW(szWindowClass, szDeveloper, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXFULLSCREEN) - WindowWidth) / 2, (GetSystemMetrics(SM_CYFULLSCREEN) - WindowHeight) / 2, WindowWidth, WindowHeight,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
