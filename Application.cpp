//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <chrono>
#include <thread>
#include "Application.h"
#include "fpscontrol.h"
#include "renderer.h"
#include "Camera.h"
#include "CDirectInput.h"

#include "Scene.h"
#include "DebugUI.h"

//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
const auto ClassName = TEXT("2024 framework ひな型");     //!< ウィンドウクラス名.
const auto WindowName = TEXT("2024 framework ひな型(フィールド描画)");    //!< ウィンドウ名.

//-----------------------------------------------------------------------------
// Class Static
//-----------------------------------------------------------------------------
HINSTANCE  Application::m_hInst;        // インスタンスハンドルです.
HWND       Application::m_hWnd;         // ウィンドウハンドルです.
uint32_t   Application::m_Width;        // ウィンドウの横幅です.
uint32_t   Application::m_Height;       // ウィンドウの縦幅です.

// ImGuiのWin32プロシージャハンドラ(マウス対応)
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
///////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//      コンストラクタです.
//-----------------------------------------------------------------------------
Application::Application(uint32_t width, uint32_t height)
{ 
    m_Height = height;
    m_Width = width;

    timeBeginPeriod(1);
}

//-----------------------------------------------------------------------------
//      デストラクタです.
//-----------------------------------------------------------------------------
Application::~Application()
{ 
    timeEndPeriod(1);
}

//-----------------------------------------------------------------------------
//      実行します.
//-----------------------------------------------------------------------------
void Application::Run()
{
    if (InitApp())
    { 
        MainLoop(); 
    }

    TermApp();
}

//-----------------------------------------------------------------------------
//      初期化処理です.
//-----------------------------------------------------------------------------
bool Application::InitApp()
{
    // ウィンドウの初期化.
    if (!InitWnd())
    { 
        return false; 
    }

    // 正常終了.
    return true;
}

//-----------------------------------------------------------------------------
//      終了処理です.
//-----------------------------------------------------------------------------
void Application::TermApp()
{
    // ウィンドウの終了処理.
    TermWnd();
}

//-----------------------------------------------------------------------------
//      ウィンドウの初期化処理です.
//-----------------------------------------------------------------------------
bool Application::InitWnd()
{
    // インスタンスハンドルを取得.
    auto hInst = GetModuleHandle(nullptr);
    if (hInst == nullptr)
    { 
        return false; 
    }

    // ウィンドウの設定.
    WNDCLASSEX wc = {};
    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = WndProc;
    wc.hIcon            = LoadIcon(hInst, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(hInst, IDC_ARROW);
    wc.hbrBackground    = GetSysColorBrush(COLOR_BACKGROUND);
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = ClassName;
    wc.hIconSm          = LoadIcon(hInst, IDI_APPLICATION);

    // ウィンドウの登録.
    if (!RegisterClassEx(&wc))
    { return false; }

    // インスタンスハンドル設定.
    m_hInst = hInst;

    // ウィンドウのサイズを設定.
    RECT rc = {};
    rc.right  = static_cast<LONG>(m_Width);
    rc.bottom = static_cast<LONG>(m_Height);

    // ウィンドウサイズを調整.
    auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    AdjustWindowRect(&rc, style, FALSE);

    // ウィンドウを生成.
    m_hWnd = CreateWindowEx(
        0,
        //        WS_EX_TOPMOST,
        ClassName,
        WindowName,
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right  - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        m_hInst,
        nullptr);

    if (m_hWnd == nullptr)
    { return false; }
 
    // ウィンドウを表示.
    ShowWindow(m_hWnd, SW_SHOWNORMAL);

    // ウィンドウを更新.
    UpdateWindow(m_hWnd);

    // ウィンドウにフォーカスを設定.
    SetFocus(m_hWnd);

    // 正常終了.
    return true;
}

//-----------------------------------------------------------------------------
//      ウィンドウの終了処理です.
//-----------------------------------------------------------------------------
void Application::TermWnd()
{
    // ウィンドウの登録を解除.
    if (m_hInst != nullptr)
    { UnregisterClass(ClassName, m_hInst); }

    m_hInst = nullptr;
    m_hWnd  = nullptr;
}

//-----------------------------------------------------------------------------
//      メインループです.
//-----------------------------------------------------------------------------
void Application::MainLoop()
{
    MSG msg = {};

    // FPS調整クラス
    FPS fpsrate(60);

    // 描画初期化
    Renderer::Init();

    // デバッグUI初期化
    DebugUI::Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

    // 入力初期化
    CDirectInput::GetInstance().Init(m_hInst,m_hWnd,m_Width,m_Height);

    // シーン初期化
    SceneInit();

    while(WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            uint64_t delta_time = 0;

            // デルタタイムを計算
            delta_time = fpsrate.CalcDelta();

            // 入力初期化
            CDirectInput::GetInstance().GetKeyBuffer();

            // 更新処理
            SceneUpdate();

            // 描画前処理
            Renderer::Begin();

            SceneDraw();

            // デバッグUI描画
            DebugUI::Render();

            // 描画後処理
            Renderer::End();

            // 規定時間までWAIT
            fpsrate.Wait();
        }
    }

    // デバッグUI初期化
    DebugUI::DisposeUI();

    // 描画終了処理
    Renderer::Uninit();
}

//-----------------------------------------------------------------------------
//      ウィンドウプロシージャです.
//-----------------------------------------------------------------------------
LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wp, lp))
        return true;

    switch(msg)
    {
        case WM_DESTROY:
            { 
                PostQuitMessage(0); 
            }
            break;

        default:
            { /* DO_NOTHING */ }
            break;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}
