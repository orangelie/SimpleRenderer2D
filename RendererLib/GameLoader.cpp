#include "pch.h"
#include "GameLoader.h"
#include "IGameInterface.h"
#include "RenderEngine.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void GameLoader::Init(HINSTANCE hInst, int width, int height, shared_ptr<IGameInterface> game)
{
    _hInst = hInst;

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = 0;
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = L"Game";
    wcex.hIconSm = 0;

    RegisterClassExW(&wcex);

    RECT rc = { 0, 0, width, height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    _hWnd = CreateWindowW(L"Game", L"Game", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, hInst, nullptr);
    assert(_hWnd);

    ShowWindow(_hWnd, SW_SHOW);
    UpdateWindow(_hWnd);

    _game = make_shared<RenderEngine>();
    _game->Start(hInst, _hWnd, width, height, game);
    INPUT->Init(_hWnd);
    TIMER->Init();
}

void GameLoader::Run()
{
    MSG msg = {};

    for (;;)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
            break;

        // IMGUI DPI Á¶Á¤
        /*{
            RECT rect;
            GetClientRect(_hWnd, &rect);

            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(
                static_cast<float>(rect.right - rect.left),
                static_cast<float>(rect.bottom - rect.top));
        }*/

        TIMER->Update();
        INPUT->Update();

        _game->Update(TIMER->GetDeltaTime());
        _game->Render();
    }
}

LRESULT CALLBACK GameLoader::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            if (GET_SINGLE(GameLoader))
            {
                if (GET_SINGLE(GameLoader)->GetEngine())
                {
                    GET_SINGLE(GameLoader)->GetEngine()->ResizeBuffer(LOWORD(lParam), HIWORD(lParam));
                }
            }
        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
