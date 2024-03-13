// DX1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "DX1.h"

#define SAFE_RELESE(x) if(x){(x)->Release(); (x)=nullptr;}

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

INT window_Width = 0;
INT window_Height = 0;
HWND gWnd;
INT img_Width = 0;
INT img_Height = 0;
IDXGISwapChain* pSwapChain = nullptr;             // the pointer to the swap chain interface
ID3D11Device* pd3dDevice = nullptr;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext* pd3dContext = nullptr;           // the pointer to our Direct3D device context
ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
ID3D11BlendState* g_pBlendState = nullptr;




D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;    //드라이버 타입
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;//드라이버 버전

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HRESULT             InitDevice();
void Render();

#include "Init_DX.h"
#include "Init_RenderTarget_ViewPort.h"
#include "Init_VSPS.h"
#include "Init_Model.h"
#include "Init_Texture.h"
#include "Init_Matrix.h"

//800 600
//700 500
float Mx =50, My = 50;
float Mw = 750, Mh = 450;
float offsetx = 400;
float offsety = 300;
float dirx = 1;
float diry = 1;

float offset = 0.f;
float dir = 0.005f;
float end = 0.5f;

void Move_WorldTM()
{
    // Update our time
    static float t = 0.0f;
    if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)XM_PI * 0.0125f;
    }
    else
    {
        static ULONGLONG timeStart = 0;
        ULONGLONG timeCur = GetTickCount64();
        if (timeStart == 0)
            timeStart = timeCur;
        t = (timeCur - timeStart) / 1000.0f;
    }
    g_World = XMMatrixRotationX(t);
}

/*
    if (g_pBlendState) {

        FLOAT blendFactor[4] = { 0.f,0.f,0.f,0.f };

        UINT mask = 0xFFFFFFFF;

        pd3dContext->OMSetBlendState(g_pBlendState, blendFactor, mask);
    }
*/
void Render()//그리기 포함
{
    pd3dContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);//랜드타겟 설정
    pd3dContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::MidnightBlue);//랜드타겟 블루로 클리어

    Render_Shader();
    Render_Texture();
    Render_Model();

    //g_World = XMMatrixIdentity();
    Move_WorldTM();
    Render_Matrix();

    pd3dContext->DrawIndexed(36, 0, 0);
    pSwapChain->Present(DXGI_SWAP_EFFECT_SEQUENTIAL, 0);//pSwapChain->Present(0, 0);
    //pd3dContext->Draw(4, 0);//버텍스 갯수, 버텍스 시작점
  

    //Sleep(10);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DX1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }//윈도우 생성
    
    MSG msg = { 0 };

    if (FAILED(InitDevice())) goto E_FINAL_POS;
    if (FAILED(Init_RenderTarget_ViewPort())) goto E_FINAL_POS;//스크린 좌표계
    if (FAILED(Init_VertexShader())) goto E_FINAL_POS;
    if (FAILED(Init_PixelShader())) goto E_FINAL_POS;
    if (FAILED(Init_Model())) goto E_FINAL_POS;
    if (FAILED(Init_Texture())) goto E_FINAL_POS;
    if (FAILED(Init_BlendState())) goto E_FINAL_POS;
    if (FAILED(Init_Matrix())) goto E_FINAL_POS;
    

    // 기본 메시지 루프입니다:
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Render();
        }
    }

E_FINAL_POS:

    SAFE_RELESE(g_pConstantBuffer);
    SAFE_RELESE(g_pIndexBuffer);

    SAFE_RELESE(g_pBlendState);

    SAFE_RELESE(g_pTextureRV);
    SAFE_RELESE(g_pSamplerLinear);

    SAFE_RELESE(g_pVertexBuffer);
    SAFE_RELESE(g_pVertexLayout);
    SAFE_RELESE(g_pVertexShader);
    SAFE_RELESE(g_pPixelShader);
    

    SAFE_RELESE(g_pRenderTargetView);
    SAFE_RELESE(pd3dContext);
    SAFE_RELESE(pSwapChain);
    SAFE_RELESE(pd3dDevice);
    
    
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DX1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   RECT rc = {0,0,800,600};
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
       rc.right-rc.left,rc.bottom-rc.top,
       nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   gWnd = hWnd;
   GetClientRect(hWnd, &rc);
   window_Width = rc.right - rc.left;
   window_Height = rc.bottom - rc.top;

   ShowWindow(hWnd, nCmdShow);
   // UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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

/*
void Render()
{
    pd3dContext->ClearRenderTargetView(g_pRenderTargetView, DirectX::Colors::MidnightBlue);

    pSwapChain->Present(0, 0);

    Sleep(10);
}
*/