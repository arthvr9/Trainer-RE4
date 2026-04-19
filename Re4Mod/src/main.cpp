#include <iostream>
#include <dwmapi.h>
#include <d3d9.h>
#include <windows.h>
#include <psapi.h>

#include "process/Process.h"
#include "memory/Memory.h"
#include "mods/InfiniteAmmo.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param))
        return true;

    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int cmd_show)
{
    // window
    WNDCLASSEX wc{};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = window_procedure;
    wc.hInstance = instance;
    wc.lpszClassName = L"ExternalOverlay";

    RegisterClassEx(&wc);

    HWND window = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        wc.lpszClassName,
        L"Overlay",
        WS_POPUP,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        nullptr, nullptr,
        instance,
        nullptr
    );

    SetLayeredWindowAttributes(window, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(window, &margins);

    ShowWindow(window, cmd_show);
    UpdateWindow(window);

    // dx9
    IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) {
        MessageBox(0, L"Erro ao criar D3D9", 0, 0);
        return 1;
    }

    D3DPRESENT_PARAMETERS d3dpp{};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth = GetSystemMetrics(SM_CXSCREEN);
    d3dpp.BackBufferHeight = GetSystemMetrics(SM_CYSCREEN);
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    IDirect3DDevice9* device = nullptr;

    if (FAILED(d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        window,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &device)))
    {
        MessageBox(0, L"Erro ao criar device DX9", 0, 0);
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);

    Process proc;
    if (!proc.Attach(L"bio4.exe")) {
        MessageBox(0, L"RE4 não encontrado", 0, 0);
        return 1;
    }

    uintptr_t base = proc.GetModuleBase(L"bio4.exe");

    Memory mem(proc.handle);
    InfiniteAmmo ammo(base, &mem);

    bool infiniteAmmo = true;
    bool menuOpen = true;

    MSG msg{};
    bool running = true;

    while (running)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                running = false;
        }

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            menuOpen = !menuOpen;

            LONG style = GetWindowLong(window, GWL_EXSTYLE);

            if (menuOpen)
            {
                SetWindowLong(window, GWL_EXSTYLE, style & ~WS_EX_TRANSPARENT);
                SetForegroundWindow(window);
            }
            else
            {
                SetWindowLong(window, GWL_EXSTYLE, style | WS_EX_TRANSPARENT);
            }
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::GetIO().MouseDrawCursor = menuOpen;

        ImGui::Begin("RE4 Trainer");

        ImGui::Checkbox("Infinite TMP Ammo", &infiniteAmmo);

        ImGui::End();

        ImGui::EndFrame();

        device->Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);

        if (device->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            device->EndScene();
        }

        device->Present(nullptr, nullptr, nullptr, nullptr);

        if (infiniteAmmo)
            ammo.Update();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (device) device->Release();
    if (d3d) d3d->Release();

    DestroyWindow(window);
    UnregisterClass(wc.lpszClassName, instance);

    return 0;
}