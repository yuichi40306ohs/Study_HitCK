#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include    <Windows.h>
#include    <cstdint>
#include    "NonCopyable.h"


///////////////////////////////////////////////////////////////////////////////
// App class
///////////////////////////////////////////////////////////////////////////////
class Application : NonCopyable
{
    //=========================================================================
    // list of friend classes and methods.
    //=========================================================================
    /* NOTHING */

public:
    //=========================================================================
    // public variables.
    //=========================================================================
    /* NOTHING */

    //=========================================================================
    // public methods.
    //=========================================================================
    Application(uint32_t width, uint32_t height);
    ~Application();
    void Run();

    // 幅を取得
    static uint32_t GetWidth() {
        return m_Width;
    }

    // 高さを取得
    static uint32_t GetHeight() {
        return m_Height;
    }

    // ウインドウハンドルを返す
    static HWND GetWindow() {
        return m_hWnd;
    }

private:
    //=========================================================================
    // private variables.
    //=========================================================================
    static HINSTANCE   m_hInst;        // インスタンスハンドルです.
    static HWND        m_hWnd;         // ウィンドウハンドルです.
    static uint32_t    m_Width;        // ウィンドウの横幅です.
    static uint32_t    m_Height;       // ウィンドウの縦幅です.

    //=========================================================================
    // private methods.
    //=========================================================================
    static bool InitApp();
    static void TermApp();
    static bool InitWnd();
    static void TermWnd();
    static void MainLoop();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
};